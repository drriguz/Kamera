#ifndef DUMMY_SINK_H
#define DUMMY_SINK_H

#include "rtsp_session.h"

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/mathematics.h>
#include <libswscale/swscale.h>
}

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

// Define a data sink (a subclass of "MediaSink") to receive the data for each subsession (i.e., each audio or video 'substream').
// In practice, this might be a class (or a chain of classes) that decodes and then renders the incoming audio or video.
// Or it might be a "FileSink", for outputting the received data into a file (as is done by the "openRTSP" application).
// In this example code, however, we define a simple 'dummy' sink that receives incoming data, but does nothing with it.

class DummySink: public MediaSink {
public:
    static DummySink* createNew(RtspSession *session,
                                UsageEnvironment& env,
                                MediaSubsession& subsession, // identifies the kind of data that's being received
                                char const* streamId = NULL); // identifies the stream itself (optional)

private:
    DummySink(RtspSession *session, UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
    // called only by "createNew()"
    virtual ~DummySink();

    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
    void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
                           struct timeval presentationTime, unsigned durationInMicroseconds);
    //void saveFrameToFile()
private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();

public:
    void setSprop(u_int8_t const* prop, unsigned size);
private:
    RtspSession* session;
private:
    u_int8_t* fReceiveBuffer;
    u_int8_t* fReceiveBufferAV;
    MediaSubsession& fSubsession;
    char* fStreamId;
private:
    AVCodec *codec;
    AVCodecContext *codecContext;
    int frameIndex;
    int got_picture;
    int len;
    AVFrame *frame;
    AVFrame *rgbFrame;
    SwsContext* sws_cxt;
    uint8_t inbuf[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE];
    char buf[1024];
    AVPacket avpkt;
};

#endif // DUMMY_SINK_H
