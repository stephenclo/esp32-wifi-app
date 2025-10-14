#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <LittleFS.h>

class WiFiController {
    public:
        // WiFi initialization and automatic connection
        void begin(String projectName);

        // Check if WiFi is connected
        bool isConnected();

        // Resets stored WiFi settings
        void resetSettings();
        
        // Callback used when entering Wifi configuration mode
        static void configModeCallback(WiFiManager *myWiFiManager);

    private:
        WiFiManager _wiFiManager;
};
