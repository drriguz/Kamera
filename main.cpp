#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>

#include <QApplication>

#include "app_window.h"

#include "rtsp_client.h"
#include "dummy_sink.h"
#include "stream_state.h"

int main(int argc, char** argv) {
    avcodec_register_all();
    av_register_all();
    QApplication app(argc, argv);
    AppWindow win(argv[0]);
    win.show();

    return app.exec();
}
