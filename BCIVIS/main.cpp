#include "mainwindow.h"

#include <QApplication>
#include "osal/osal_log.h"

# define TAG "MAIN"
int main(int argc, char *argv[])
{
    setvbuf(stdout, 0, _IONBF, 0);//控制台输出不缓存，便于实时调试
    OSAL_LOG(TAG,"Begin");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
