#include "decode_thread.h"
#include <iostream>

DecodeThread::DecodeThread(QObject *parent)
    :running(true),
      QThread(parent)
{

}

DecodeThread::~DecodeThread()
{
    stop();
    quit();
    wait();
}

void DecodeThread::run(){
    while(this->running){
        std::cout << "->" << std::endl;
        sleep(3);
    }
}

void DecodeThread::stop(){
    this->running = false;
}

