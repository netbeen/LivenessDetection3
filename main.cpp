#include "mainwindow.h"
#include <QApplication>
#include <Controller.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType<BoundingBox>("BoundingBox");
    qRegisterMetaType<cv::Mat_<double>>("cv::Mat_<double>");

    MainWindow w;
    w.show();

    return a.exec();
}
