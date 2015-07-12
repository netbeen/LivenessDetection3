#include "Controller.h"

//单例模式的静态指针的定义+初始化
Controller* Controller::ptr2Controller = nullptr;

Controller::Controller():currentAnalyseIndex(-1)
{
    faceAligner = FaceAligner::getInstance();       //提前加载faceAligner
    faceAlignerThread = new QThread();
    faceAligner->moveToThread(faceAlignerThread);
    faceAlignerThread->start();

    analyserFactory = new AnalyserFactory();
    QThread::connect(this,SIGNAL(startNextAnalyserSignal()),this,SLOT(startNextAnalyserSlot()));    //由Controller自行调用自身的slot
    webcamCapture = WebcamCapture::getInstance();
    webcamThread = new QThread();
    webcamCapture->moveToThread(webcamThread);
    webcamThread->start();
}

//单例模式创建函数
Controller* Controller::getInstance(){
    if(Controller::ptr2Controller == nullptr){
        Controller::ptr2Controller = new Controller();
    }
    return Controller::ptr2Controller;
}

void Controller::start(){
    std::cout << "Controller at " << QThread::currentThreadId()<< std::endl;
    analyserOrder.clear();
    for(std::string elemStr : analyserFactory->analyserType){
        this->analyserOrder.push_back(elemStr);
    }
    Utils::randomizeVector(this->analyserOrder);
    std::cout << "------------------------" << std::endl;
    for(std::string elemStr : this->analyserOrder){
        std::cout << elemStr << std::endl;
    }
    std::cout << "------------------------" << std::endl;
    emit this->startNextAnalyserSignal();
}

void Controller::startNextAnalyserSlot(){
    this->currentAnalyseIndex++;
    //std::cout <<  "this->currentAnalyseIndex="<<this->currentAnalyseIndex  << std::endl;
    if(this->currentAnalyseIndex < this->analyserOrder.size()){
        Analyser* analyser = AnalyserFactory::createAnalyser(this->analyserOrder.at(this->currentAnalyseIndex));
        QObject::connect(this,SIGNAL(analyserStartSignal()),analyser,SLOT(start()));
        analyserVector.push_back(analyser);

        analyserThread = new QThread();
        analyser->moveToThread(analyserThread);
        QObject::connect(analyser,SIGNAL(done(bool)),this,SLOT(receiveAnalyserResultSlot(bool)));
        analyserThread->start();
        emit this->analyserStartSignal();
        QObject::disconnect(this,SIGNAL(analyserStartSignal()),analyser,SLOT(start()));

    }else{
        std::cout << "All over" << std::endl;
    }
}

void Controller::receiveAnalyserResultSlot(bool result){
    if(result == true){
        emit this->startNextAnalyserSignal();
    }else{
        std::cout << "Controller deny" <<std::endl;
    }
}
