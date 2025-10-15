#pragma once
#include <Arduino.h>
#include <LittleFS.h>
#include "FS.h"

class RandomSensorController {
    public:
        // Must be called in loop()
        void readSensorLoop();

        void printLogFile();
    private:
        unsigned long _lastActionTime;
        void appendCircularLog(const char* data);
        float readSensorValue();
};
