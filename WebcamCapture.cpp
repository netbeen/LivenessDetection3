#include "WebcamCapture.h"

WebcamCapture* WebcamCapture::ptr2WebcamCapture = nullptr;

WebcamCapture::WebcamCapture()
{
    if (cap.open(0)) {
        cap.set(CV_CAP_PROP_FRAME_WIDTH,800);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,600);
        cap.set(CV_CAP_PROP_FPS, 30);
    }else{
        exit(2);
    }
}

WebcamCapture* WebcamCapture::getInstance(){
    if(WebcamCapture::ptr2WebcamCapture == nullptr){
        WebcamCapture::ptr2WebcamCapture = new WebcamCapture();
    }
    return WebcamCapture::ptr2WebcamCapture;
}

WebcamCapture::~WebcamCapture()
{
    if(cap.isOpened()){
        cap.release();
    }
}

void WebcamCapture::start(){
    forever{
        cv::Mat rawImg;
        cap >> rawImg;
        //cv::cvtColor(rawImg,rawImg,cv::COLOR_BGR2RGB);      //转换为RGB格式
        emit newImageCaptured(rawImg);
    }
}
