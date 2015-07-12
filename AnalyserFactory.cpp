#include "AnalyserFactory.h"

AnalyserFactory::AnalyserFactory()
{
    analyserType.insert("blink");
    //analyserType.insert("yaw");
    analyserType.insert("openMouth");
}

Analyser* AnalyserFactory::createAnalyser(std::string inputType){
    Analyser* newAnalyser;

    if(inputType.compare("blink") == 0){
        newAnalyser = new BlinkAnalyser();
    }else if(inputType.compare("yaw") == 0){
        //newAnalyser = new YawAnalyser();
    }else if(inputType.compare("openMouth") == 0){
        newAnalyser = new OpenMouthAnalyser();
    }

    return newAnalyser;
}
