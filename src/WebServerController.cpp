#include "WebServerController.h"

// Constructor
WebServerController::WebServerController(int port)
  : _server(port) {}
  
// Serve web app files through the web server
void WebServerController::handleFile(String path) {
  
  Serial.println("handleFile" + path);
  if (path.endsWith("/")) path += "index.html"; // default page

  String contentType = "text/plain";
  if (path.endsWith(".html")) contentType = "text/html";
  else if (path.endsWith(".css")) contentType = "text/css";
  else if (path.endsWith(".js")) contentType = "application/javascript";
  else if (path.endsWith(".png")) contentType = "image/png";
  else if (path.endsWith(".jpg")) contentType = "image/jpeg";
  else if (path.endsWith(".gif")) contentType = "image/gif";
  else if (path.endsWith(".svg")) contentType = "image/svg+xml";
  else if (path.endsWith(".ico")) contentType = "image/x-icon";

  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    _server.streamFile(file, contentType);
    file.close();
  } else {
    _server.send(404, "text/plain", "404: File Not Found");
  }
}

// Function to reset wifi settings via web server
void WebServerController::handleResetWifi() {
  _server.send(200, "text/html", "Reset Wifi");
  _wiFiController.resetSettings();
}

// Function to serve the web page
void WebServerController::handleIsConnected() {
  _server.send(200, "text/html", _wiFiController.isConnected() ? "true" : "false");
}

// Serve POST to set refresh interval value
void WebServerController::handleSetRefreshInterval() {
  if (_server.hasArg("plain")) {
    String body = _server.arg("plain");
    char* endPtr;

    unsigned long value = strtoul(body.c_str(), &endPtr, 10);
 
    if (*endPtr != '\0') {
      Serial.println("Invalid number!");
    } else {
      _randomSensorController.setRefreshInterval(value);
    }
    _server.send(200, "text/html");
  } else {
    _server.send(400, "text/html");
  }
}
        
// Serve get refresh interval value 
void WebServerController::handleGetRefreshInterval(){
  unsigned long refresgInterval = _randomSensorController.getRefreshInterval();
  _server.send(200, "text/html", String(refresgInterval));
}

void WebServerController::handleSensorValues() {
    // Get vector of 5 last sensor values
    std::array<float, 5> data = _randomSensorController.getSensorValues();

    // Create the JSON document
    JsonDocument doc;

    // Create JSON Array from Vector C++
    JsonArray array = doc.to<JsonArray>();

    for (const float& value : data) {
      array.add(value);
    }

    // Serialize the document to a String
    String output;
    serializeJson(doc, output);

    // Response
    _server.send(200, "application/json", output);
}

// Start the server and configure the routes
void WebServerController::begin() {
  Serial.println("🚀 Server Web initialisation...");

  // is-connected route
  _server.on("/is-connected", std::bind(&WebServerController::handleIsConnected, this));

  // reset-wifi route
  _server.on("/reset-wifi", std::bind(&WebServerController::handleResetWifi, this));

  // reset-wifi route
  _server.on("/sensor-values", std::bind(&WebServerController::handleSensorValues, this));

  _server.on("/refresh-interval", HTTP_POST, std::bind(&WebServerController::handleSetRefreshInterval, this));

  _server.on("/refresh-interval", HTTP_GET, std::bind(&WebServerController::handleGetRefreshInterval, this));

  // Generic route
  _server.onNotFound([this]() {
    this->handleFile(this->_server.uri());
  });

  _server.begin();
  Serial.println("✅ Web Server started !");
}

// Must be called regularly in loop()
void WebServerController::handleClient() {
  _server.handleClient();
}
