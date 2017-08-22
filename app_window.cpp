#include "app_window.h"

WCwindow::WCwindow(const char* progName)
    : _progName(progName)
{
    QGridLayout *layout = new QGridLayout;

    this->_cameraScreen = new QLabel("Video 0");
    this->_cameraScreen->setFixedSize(400, 300);
    this->_cameraScreen->setStyleSheet("background:'black';color:'white'");
    layout->addWidget(this->_cameraScreen, 0, 0);

    QWidget * central = new QWidget(this);

    setCentralWidget(central);
    centralWidget()->setLayout(layout);

    setWindowTitle("Camera Window");
    setFixedSize(800, 600);

    this->_rtspSession = new RtspSession(this->_progName.c_str(), "rtsp://172.16.1.183:8554/txdx.mkv", this);
    this->_rtspSession->start();

}

WCwindow::~WCwindow()
{

}
