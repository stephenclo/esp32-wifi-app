

#include <Arduino.h>
#include <LittleFS.h> // Librairie du system de fichier

#include "WiFiController.h"
#include "WebServerController.h"

// Change Project Name, Access Point SSID  
String projectName = "My Project";

WiFiController wiFiController;
WebServerController webServerController;

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
  
  Serial.print("✅ Project started");
}

void loop() {
  // Listen for and respond to web requests
  webServerController.handleClient();
}