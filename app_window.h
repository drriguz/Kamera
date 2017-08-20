#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QImage>

#include "decode_thread.h"

class WCwindow : public QMainWindow
{
public:
    WCwindow();
    ~WCwindow();
private:
    QLabel* _cameraScreen;
    DecodeThread* _decodeThread;
};


#endif // APP_WINDOW_H
