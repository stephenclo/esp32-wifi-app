#include <Arduino.h>
#include "LittleFS.h"
#include "WiFiController.h"
#include "WebServerController.h"
#include "RandomSensorController.h"

// Change Project Name, Access Point SSID  
String projectName = "My Project";

WiFiController wiFiController;
WebServerController webServerController;
RandomSensorController randomSensorController;

void setup() {
  Serial.print("🚀 Project launch");
  Serial.setDebugOutput(true);
  Serial.begin(115200);
  delay(1000);

   // Mout file system 
  if (!LittleFS.begin()) {
    Serial.println("⚠️ Error: file system not mounted !");
    return;
  }

  // Start wifi controller
  wiFiController.begin(projectName);

  // Start web server controller
  webServerController.begin();

  // Start random sensor controller
  randomSensorController.begin();
  
  Serial.println("✅ Project started");
}

void loop() {
  // Listen for and respond to web requests
  webServerController.handleClient();

  randomSensorController.readSensorLoop();
  
  // Wait 1 second before next reading
  delay(1000);
}