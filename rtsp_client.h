#ifndef CAMERA_RTSP_CLIENT_H
#define CAMERA_RTSP_CLIENT_H

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>

#include "stream_state.h"
#include "rtsp_session.h"

// If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
// "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
// showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
// structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

class CustomRTSPClient: public RTSPClient {
public:
    static CustomRTSPClient* createNew(
            RtspSession *session,
            UsageEnvironment& env,
            char const* rtspURL,
            int verbosityLevel = 0,
            char const* applicationName = NULL,
            portNumBits tunnelOverHTTPPortNum = 0);

protected:
    CustomRTSPClient(RtspSession *session,
                     UsageEnvironment& env,
                     char const* rtspURL,
                     int verbosityLevel,
                     char const* applicationName,
                     portNumBits tunnelOverHTTPPortNum);
    // called only by createNew();
    virtual ~CustomRTSPClient();
public:
    StreamClientState& getStreamClientState() { return this->streamClientState; }
    RtspSession* getSession() { return this->session; }
private:
    RtspSession* session;
    StreamClientState streamClientState;
};


#endif // CAMERA_RTSP_CLIENT_H
