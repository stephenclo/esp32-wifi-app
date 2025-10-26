#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#include <WebServer.h>
#include <LittleFS.h>

#include "WiFiController.h"
#include "RandomSensorController.h"

class WebServerController {
    public:
        // Constructor with port parameter
        WebServerController(int port = 80);

        // Init web server
        // Must be called in setup()
        void begin();
        
        // Reads and processes requests, then sends the responses.
        // Must be called in loop()
        void handleClient();
        
    private:
        WebServer _server;
        WiFiController _wiFiController;
        RandomSensorController _randomSensorController;

        // Serve web app files through the web server
        void handleFile(String path);

        // Serve WiFi reset action
        void handleResetWifi();

        // Serve get data isConnected  
        void handleIsConnected();

        // Serve sensor values history 
        void handleSensorValues();
        
        // Serve set refresh interval value 
        void handleSetRefreshInterval();
        
        // Serve get refresh interval value 
        void handleGetRefreshInterval();
};
