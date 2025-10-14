#include "WebServerController.h"

// Constructeur : initialise le serveur sur le port donné
WebServerController::WebServerController(int port)
  : _server(port) {}
  
// Serve web app files through the web server
void WebServerController::handleFile(String path) {
  
  Serial.println("handleFile" + path);
  if (path.endsWith("/")) path += "index.html"; // page par défaut

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

// Fonction pour réinitialiser les paramètre wifi via le serveur web
void WebServerController::handleResetWifi() {
  _server.send(200, "text/html", "Reset Wifi");
  _wiFiController.resetSettings();
}

// Fonction pour servir la page web
void WebServerController::handleIsConnected() {
  _server.send(200, "text/html", _wiFiController.isConnected() ? "true" : "false");
}

// Démarre le serveur et configure les routes
void WebServerController::begin() {
  Serial.println("🚀 Initialisation du serveur web...");

  // Route is-connected
  _server.on("/is-connected", std::bind(&WebServerController::handleIsConnected, this));

  // Route reset-wifi
  _server.on("/reset-wifi", std::bind(&WebServerController::handleResetWifi, this));

  // Route générique
  _server.onNotFound([this]() {
    this->handleFile(this->_server.uri());
  });

  _server.begin();
  Serial.println("✅ Serveur web démarré !");
}

// Doit être appelée régulièrement dans loop()
void WebServerController::handleClient() {
  _server.handleClient();
}
