#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <WebcamCapture.h>
#include <Controller.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    WebcamCapture* webcamCapture;
    Controller* controller;

signals:
    void startToRunButtonClicked();

public slots:
    void updateImage(cv::Mat newImage);

private slots:
    void on_startToRunButton_clicked();
};

#endif // MAINWINDOW_H
