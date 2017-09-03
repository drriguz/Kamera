#include "app_window.h"
#include <iostream>

AppWindow::AppWindow(const char* progName, QWidget* parent)
    : QMainWindow(parent),
      _progName(progName){
    this->setupUi();
    this->bindEvents();
    this->_rtspSession = new RtspSession(this->_progName.c_str(), "rtsp://172.16.1.183:8554/344.mkv", this);
}

void AppWindow::setupUi(){
    QGridLayout *layout = new QGridLayout;

    this->_cameraScreen = new QLabel("Video 0", this);
    this->_cameraScreen->setFixedSize(400, 300);
    this->_cameraScreen->setStyleSheet("background:'black';color:'white'");

    this->_startCaptureButton = new QPushButton(this);
    this->_startCaptureButton->setText("Start");

    this->_stopCaptureButton = new QPushButton(this);
    this->_stopCaptureButton->setText("Stop");

    layout->addWidget(this->_startCaptureButton, 0, 0);
    layout->addWidget(this->_stopCaptureButton, 1, 0);
    layout->addWidget(this->_cameraScreen, 2, 0);

    QWidget * central = new QWidget(this);

    setCentralWidget(central);
    centralWidget()->setLayout(layout);

    setWindowTitle("Camera Window");
    setFixedSize(800, 600);
}

void AppWindow::bindEvents(){
    connect(this->_startCaptureButton, SIGNAL(clicked()), this, SLOT(onStart()));
    connect(this->_stopCaptureButton, SIGNAL(clicked()), this, SLOT(onStop()));
}

void AppWindow::onStart(){
    std::cout << "starting..." << std::endl;
    this->_rtspSession->start();
}

void AppWindow::onStop(){
    std::cout << "stoping..." << std::endl;
    this->_rtspSession->stop();
}
