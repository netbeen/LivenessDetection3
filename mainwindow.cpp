#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    webcamCapture = WebcamCapture::getInstance();
    QObject::connect(webcamCapture,SIGNAL(newImageCaptured(cv::Mat)),this,SLOT(updateImage(cv::Mat)));

    controller = Controller::getInstance();
    QObject::connect(this,SIGNAL(startToRunButtonClicked()),controller,SLOT(start()));

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(cv::Mat newImage){
    QImage displayImage = QImage( static_cast<const unsigned char*>(newImage.data), newImage.cols, newImage.rows, QImage::Format_RGB888 );
    ui->imageLabel->setPixmap( QPixmap::fromImage(displayImage) );
}

void MainWindow::on_startToRunButton_clicked()
{
    emit startToRunButtonClicked();
}
