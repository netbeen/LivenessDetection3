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
    QObject::connect(controller,SIGNAL(updateSlider(int)),this,SLOT(receiveSliderPercentage(int))); //从controller传来的滑块percentage

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(cv::Mat newImage){
    cv::cvtColor(newImage,newImage,cv::COLOR_BGR2RGB);      //修正BGR显示模式为RGB
    QImage displayImage = QImage( static_cast<const unsigned char*>(newImage.data), newImage.cols, newImage.rows, QImage::Format_RGB888 );
    ui->imageLabel->setPixmap( QPixmap::fromImage(displayImage) );
}

void MainWindow::on_startToRunButton_clicked()
{
    emit startToRunButtonClicked();
}

void MainWindow::receiveSliderPercentage(int percentage){
    this->ui->horizontalSlider->setValue(percentage);
}
