#include "rtsp_client.h"

// Implementation of "ourRTSPClient":

CustomRTSPClient* CustomRTSPClient::createNew(
        int id,
        UsageEnvironment& env,
        char const* rtspURL,
        int verbosityLevel,
        char const* applicationName,
        portNumBits tunnelOverHTTPPortNum) {
    return new CustomRTSPClient(id, env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

CustomRTSPClient::CustomRTSPClient(
        int id,
        UsageEnvironment& env,
        char const* rtspURL,
        int verbosityLevel,
        char const* applicationName,
        portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) ,
      clientId(id){
}

CustomRTSPClient::~CustomRTSPClient() {
}
