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
        // Must be called in loop()
        void readSensorLoop();

        // Get history of all data
        std::array<float, 5> getSensorValues();

    private:
        unsigned long _lastActionTime;
        void saveLastSensorValue(float value);
        float readSensorValue();
};
