#include "rtsp_client.h"

// Implementation of "ourRTSPClient":

CustomRTSPClient* CustomRTSPClient::createNew(
        RtspSession *session,
        UsageEnvironment& env,
        char const* rtspURL,
        int verbosityLevel,
        char const* applicationName,
        portNumBits tunnelOverHTTPPortNum) {
    return new CustomRTSPClient(session, env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

CustomRTSPClient::CustomRTSPClient(
        RtspSession *session,
        UsageEnvironment& env,
        char const* rtspURL,
        int verbosityLevel,
        char const* applicationName,
        portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) ,
      session(session){
}

CustomRTSPClient::~CustomRTSPClient() {
}
