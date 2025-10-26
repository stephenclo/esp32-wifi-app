#pragma once
#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <vector>
#include <string>
#include <LittleFS.h>
#include "FS.h"

class RandomSensorController {
    public:
        // Must be called in begin()    
        void begin();

        // Must be called in loop()
        void readSensorLoop();

        // Get history of all data
        std::array<float, 5> getSensorValues();

        // Record last sensor values input on NVS (Keep last 5 sensor values)
        void saveLastSensorValue(float value);

        unsigned long getRefreshInterval();

        void setRefreshInterval(unsigned long value);
    private:
        unsigned long _lastActionTime;
        float readSensorValue();
};
