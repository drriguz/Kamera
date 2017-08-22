#include <liveMedia/liveMedia.hh>
#include <BasicUsageEnvironment/BasicUsageEnvironment.hh>

#include <QApplication>

#include "app_window.h"

#include "rtsp_client.h"
#include "dummy_sink.h"
#include "stream_state.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    WCwindow win(argv[0]);
    win.show();


    avcodec_register_all();
    av_register_all();

    return app.exec();

    // If you choose to continue the application past this point (i.e., if you comment out the "return 0;" statement above),
    // and if you don't intend to do anything more with the "TaskScheduler" and "UsageEnvironment" objects,
    // then you can also reclaim the (small) memory used by these objects by uncommenting the following code:
    /*
    env->reclaim(); env = NULL;
    delete scheduler; scheduler = NULL;
  */
}
