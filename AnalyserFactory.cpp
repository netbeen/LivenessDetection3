#include "AnalyserFactory.h"

AnalyserFactory::AnalyserFactory()
{
    analyserType.insert("blink");
    //analyserType.insert("yaw");
    //analyserType.insert("openMouth");
}

Analyser* AnalyserFactory::createAnalyser(std::string inputType){
    Analyser* newAnalyser;

    if(inputType.compare("blink") == 0){
        newAnalyser = new BlinkAnalyser();
    }

    return newAnalyser;
}
