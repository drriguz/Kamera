#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QImage>

class WCwindow : public QMainWindow
{
public:
    WCwindow();
private:
    QLabel* _cameraScreen;
};


#endif // APP_WINDOW_H
