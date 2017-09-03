#ifndef RSTP_CLIENT_H
#define RSTP_CLIENT_H

#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>

#include <string>
#include <QThread>

class RtspSession: public QThread{
    Q_OBJECT
public:
    explicit RtspSession(int id, const char* progName, const char* url, QObject *parent =NULL);
    virtual ~RtspSession();
public:
    void stop();
signals:
    void gotFrame(const QImage& image);
protected:
    bool openURL(UsageEnvironment& env, const char* progName, const char * url);
    void run();
private:
    int _id;
    std::string _progName;
    std::string _url;
    RTSPClient* _rtspClient;
    char _eventLoopWatchVariable;
};

#endif // RSTP_CLIENT_H
