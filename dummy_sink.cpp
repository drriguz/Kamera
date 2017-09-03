#include "dummy_sink.h"

#include <Qimage>
#include <QImageWriter>
#include <iostream>

// Implementation of "DummySink":

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId) {
    return new DummySink(env, subsession, streamId);
}

DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
    : MediaSink(env),
      frameIndex(0),
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

    codecContext = avcodec_alloc_context3(codec);
    frame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    avpicture_alloc( ( AVPicture *) rgbFrame, AV_PIX_FMT_RGB24, 720, 480);

    if (codec->capabilities & CODEC_CAP_TRUNCATED) {
        codecContext->flags |= CODEC_FLAG_TRUNCATED; // we do not send complete frames
    }

    codecContext->width = 720;
    codecContext->height = 480;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(codecContext,codec,NULL) < 0) {
        envir() << "could not open codec";
        exit(5);
    }

    this->sws_cxt = sws_getContext( codecContext->width, codecContext->height, codecContext->pix_fmt, 400, 300,
                                    AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
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

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                                  struct timeval presentationTime, unsigned /*durationInMicroseconds*/) {
    // We've just received a frame of data.  (Optionally) print out information about it:
    if (strcmp(fSubsession.codecName(),"H264") == 0) {
        avpkt.data = fReceiveBufferAV;
        avpkt.size = frameSize + 4;
        memcpy (fReceiveBufferAV + 4, fReceiveBuffer, frameSize);
        avpkt.data = fReceiveBufferAV;
        len = avcodec_decode_video2 (codecContext, frame, &got_picture, &avpkt);
        if (len < 0) {
            envir() << "Error while decoding frame " << frameIndex << "\n";
        }
        if (got_picture) {
            envir() << "->Picture decoded :" << frameIndex << "\n";
            sws_scale(this->sws_cxt, this->frame->data,this->frame->linesize,
                      0, this->codecContext->height,
                      this->rgbFrame->data, this->rgbFrame->linesize);
            QImage *image = new QImage(this->rgbFrame->data[0],
                    this->codecContext->width,
                    this->codecContext->height,
                    QImage::Format_RGB888);
            QString name = QString("/Users/hfli/Movies/%1.jpg").arg(frameIndex);
            if(frameIndex % 10 == 0){
                QImageWriter writer(name);
                if(!writer.write(*image))
                {
                    std::cout << writer.errorString().toStdString();
                }
                std::cout << "saved:" << std::endl;
            }
            frameIndex ++;
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
    len = avcodec_decode_video2 (codecContext, frame, &got_picture, &avpkt);
    if (len < 0) {
        envir() << "Error while decoding frame" << frame;
        //		exit(6);
    }

    envir() << "after setSprop\n";
    //	exit (111);
}
