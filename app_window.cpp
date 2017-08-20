#include "app_window.h"

WCwindow::WCwindow()
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
}
