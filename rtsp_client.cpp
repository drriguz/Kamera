#include "rtsp_client.h"

// Implementation of "ourRTSPClient":

CustomRTSPClient* CustomRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL,
                                        int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) {
    return new CustomRTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

CustomRTSPClient::CustomRTSPClient(UsageEnvironment& env, char const* rtspURL,
                             int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
    : RTSPClient(env,rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) {
}

CustomRTSPClient::~CustomRTSPClient() {
}
