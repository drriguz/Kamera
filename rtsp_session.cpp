#include "rtsp_session.h"
#include "rtsp_client.h"

#include <iostream>

#define RTSP_CLIENT_VERBOSITY_LEVEL 1

RtspSession::RtspSession(const char* progName, UsageEnvironment& env)
    :_progName(progName),
      _url(""),
      _env(&env),
      _eventLoopWatchVariable(0),
      _running(true){
    scheduler = BasicTaskScheduler::createNew();
}

RtspSession::~RtspSession(){

}

void RtspSession::openURL(const char *rtspURL){
    RTSPClient* rtspClient = CustomRTSPClient::createNew(*env, rtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, this->_progName);
    if (rtspClient == NULL) {
        std::cerr << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env->getResultMsg() << "\n";
        return;
    }
    rtspClient->sendDescribeCommand(continueAfterDESCRIBE);
}

