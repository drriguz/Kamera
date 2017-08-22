#include "dummy_sink.h"



// Implementation of "DummySink":

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId) {
    return new DummySink(env, subsession, streamId);
}

DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
    : MediaSink(env),
      fSubsession(subsession) {
    fStreamId = strDup(streamId);
    fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
    fReceiveBufferAV = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE + 4];

    fReceiveBufferAV[0] = 0;
    fReceiveBufferAV[1] = 0;
    fReceiveBufferAV[2] = 0;
    fReceiveBufferAV[3] = 1;

    av_init_packet(&avpkt);
    avpkt.flags |= AV_PKT_FLAG_KEY;
    avpkt.pts = avpkt.dts = 0;

    memset(inbuf + INBUF_SIZE, 0, FF_INPUT_BUFFER_PADDING_SIZE);

    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        envir() << "codec not found!";
        exit(4);
    }

    c = avcodec_alloc_context3(codec);
    picture = av_frame_alloc();

    if (codec->capabilities & CODEC_CAP_TRUNCATED) {
        c->flags |= CODEC_FLAG_TRUNCATED; // we do not send complete frames
    }

    c->width = 640;
    c->height = 360;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(c,codec,NULL) < 0) {
        envir() << "could not open codec";
        exit(5);
    }
}

DummySink::~DummySink() {
    delete[] fReceiveBuffer;
    delete [] fReceiveBufferAV;
    delete[] fStreamId;
}

void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned durationInMicroseconds) {
    DummySink* sink = (DummySink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1
#define DEBUG_PRINT_NPT 1

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
    // We've just received a frame of data.  (Optionally) print out information about it:
#ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME
    if (fStreamId != NULL)
        envir() << "Stream \"" << fStreamId << "\"; ";
    envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
    if (numTruncatedBytes > 0)
        envir() << " (with " << numTruncatedBytes << " bytes truncated)";
    char uSecsStr[6+1]; // used to output the 'microseconds' part of the presentation time
    sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
    envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;
    if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
        envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
    }
#ifdef DEBUG_PRINT_NPT
    envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
#endif
    envir() << "\n";
#endif
    if (strcmp(fSubsession.codecName(),"H264") == 0) {
        avpkt.data = fReceiveBufferAV;
        avpkt.size = frameSize + 4;
        memcpy (fReceiveBufferAV + 4, fReceiveBuffer, frameSize);
        avpkt.data = fReceiveBufferAV;
        len = avcodec_decode_video2 (c, picture, &got_picture, &avpkt);
        if (len < 0) {
            envir() << "Error while decoding frame " << frame << "\n";
        }
        if (got_picture) {
            envir() << "->Picture decoded" << frame << "\n";
            frame ++;
        }
    }

    // Then continue, to request the next frame of data:
    continuePlaying();
}

Boolean DummySink::continuePlaying() {
    if (fSource == NULL) return False; // sanity check (should not happen)

    // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
    fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
                          afterGettingFrame, this,
                          onSourceClosure, this);
    return True;
}

void DummySink::setSprop(u_int8_t const* prop, unsigned size) {
    uint8_t *buf;
    uint8_t *buf_start;
    buf = (uint8_t *)malloc(1000);
    buf_start = buf + 4;

    avpkt.data = buf;
    avpkt.data[0]   = 0;
    avpkt.data[1]   = 0;
    avpkt.data[2]   = 0;
    avpkt.data[3]   = 1;
    memcpy (buf_start, prop, size);
    avpkt.size = size + 4;
    len = avcodec_decode_video2 (c, picture, &got_picture, &avpkt);
    if (len < 0) {
        envir() << "Error while decoding frame" << frame;
        //		exit(6);
    }

    envir() << "after setSprop\n";
    //	exit (111);
}
