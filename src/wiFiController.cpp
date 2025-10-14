#include "WiFiController.h"
#include "utils.h"

// Callback used when entering Wifi configuration mode
void WiFiController::configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Wifi Configuration Mode (Access Point)");
  Serial.print("Access Point SSID : ");
  Serial.println(myWiFiManager->getConfigPortalSSID());
  Serial.print("Access Point IP : ");
  Serial.println(WiFi.softAPIP());
}

// Init WiFi:
// - in Access Point mode if no network is registered
// - by connecting to the network if its credentials are registered
void WiFiController::begin(String projectName) {
  // Optional: Set the time before reboot if connection fails
  _wiFiManager.setTimeout(180); // 3 minutes

  // Define the callback to indicate that you are in Access Point mode (Wifi config)
  _wiFiManager.setAPCallback(configModeCallback);

  // Custom styles for WiFiManager interface (reuse the styles.css from the main project interface)
  File f = LittleFS.open("/styles.css", "r");
  String css = "<style>" + f.readString() + "</style>";
  _wiFiManager.setCustomHeadElement(css.c_str());
  f.close();

  // Set the SSID of the temporary Access Point
  String apName = toKebabCase(projectName) + "-config";

  Serial.println("Attempting self-connection... (3s max)");

  // connection attempt. If unsuccessful, activate Access Point mode
  if (!_wiFiManager.autoConnect(apName.c_str())) {
    Serial.println("Login failed. The configuration portal is active.");
  } else {
    Serial.println("Connected to WiFi !");
    Serial.print("IP : ");
    Serial.println(WiFi.localIP());
  }
}

// Reset the Wifi settings and restart the ESP in Access Point mode 
void WiFiController::resetSettings() {
  Serial.print("⚠️ Resetting WiFi settings !");
  _wiFiManager.resetSettings();
  ESP.restart();
}

bool WiFiController::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}