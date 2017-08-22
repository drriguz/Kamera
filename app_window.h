#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QImage>

#include <string>

#include "rtsp_session.h"

class WCwindow : public QMainWindow
{
public:
    WCwindow(const char* progName);
    ~WCwindow();
private:
    std::string _progName;
    QLabel* _cameraScreen;
    RtspSession* _rtspSession;
};


#endif // APP_WINDOW_H
