#ifndef RSTP_CLIENT_H
#define RSTP_CLIENT_H

#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>

#include <string>
#include <QThread>

class RtspSession: public QThread{
    Q_OBJECT
public:
    RtspSession(const char* progName, UsageEnvironment& env);
    virtual ~RtspSession();
public:
    void openURL(const char* rtspURL);
private:
    std::string _progName;
    std::string _url;
    TaskScheduler* _scheduler;
    UsageEnvironment* _env;
    RTSPClient* _rtspClient;
    char _eventLoopWatchVariable;
    volatile bool _running;
};

#endif // RSTP_CLIENT_H
