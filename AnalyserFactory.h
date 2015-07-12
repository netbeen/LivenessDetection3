#ifndef ANALYSERFACTORY_H
#define ANALYSERFACTORY_H

#include <BlinkAnalyser.h>
#include <OpenMouthAnalyser.h>

class AnalyserFactory
{
public:
    AnalyserFactory();
    std::set<std::string> analyserType;
    static Analyser* createAnalyser(std::string);
};

#endif // ANALYSERFACTORY_H
