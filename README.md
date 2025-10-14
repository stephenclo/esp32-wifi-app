# ESP32-WIFI-APP

## 💡 Description

A ready-to-use ESP32 boilerplate project including:
* Wi-Fi configuration via access point using tzapu/WiFiManager
* A built-in web server serving a web interface to:
  - Display ESP32 and sensor information
  - Trigger actions in the Arduino code directly from the browser
* LittleFS integration for managing and serving HTML, CSS, and JavaScript files separately from the Arduino source.

## ⚙️ Requirements

PlatformIO
ESP32 board (e.g. ESP32 DevKit, WROVER, WROOM, etc.)
Libraries (auto-installed via PlatformIO):
* tzapu/WiFiManager
* WebServer
* littlefs

## 🧩Setup

Clone this repository
* `git clone https://github.com//stephenclo/esp32-wifi-appp.git`
* `cd esp32-wifi-app`

Upload the filesystem
* `pio run --target uploadfs`

Build and flash the firmware
* `pio run --target upload`

Open Serial Monitor
* `pio device monitor`

Connect to the ESP32’s access point (default: ESP32_AP_Config)
Then configure your Wi-Fi network through the captive portal.

## 🌐 Web Interface

Once connected to WiFi, open your browser and go to the ESP32’s IP address.
You’ll see a web dashboard showing real-time data and control options.
The source files are placed in the LittleFSrepertory `/data`.   

Example routes:
| Path | Description |
|--|--|
| / | Main interface (HTML UI) |
| /is-connected |	Returns ESP32 WiFi connection state |
| /reset-wifi | Executes an action in code to reset WiFi settings |

## 🧠 Technologies Used

* C++ / Arduino Framework
* AsyncTCP + ESPAsyncWebServer for non-blocking HTTP requests
* LittleFS for web file storage
* WiFiManager for access-point-based configuration (with UI customisation)

## 🪛 Customization

You can add:
* Custom routes (e.g. /api/sensor)
* Dynamic HTML elements or AJAX calls
* Your own HTML, CSS, JS and images under `/data/` (or modify existing)

Then run `pio run --target uploadfs` to re-upload the web files.

## 📋 TODO

* Storage : Data recording 
* Simple Data viz

## 📝 License

This project is licensed under the MIT License.
Feel free to use, modify, and distribute.