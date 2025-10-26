#include "RandomSensorController.h"

const char* NVS_NAMESPACE = "TEMP_DATA"; 
const char* NVS_KEY_1 = "TEMP_1";
const char* NVS_KEY_2 = "TEMP_2";
const char* NVS_KEY_3 = "TEMP_3";
const char* NVS_KEY_4 = "TEMP_4";
const char* NVS_KEY_5 = "TEMP_5";
const char* NVS_KEY_REFRESH_INTERVAL = "REFRESH";

// 1 hour in milliseconds: 1 hour * 60 min * 60 sec * 1000 ms = 3,600,000 ms
// Using 'UL' (unsigned long) is crucial to ensure proper handling of large numbers.
unsigned long refrehInterval = 1800000UL; 

// Variable to store the last time the action was executed
unsigned long _lastActionTime = 0;

// Simulate sensor reading (replace with your actual code)
float RandomSensorController::readSensorValue() {
  // Simulates a temperature reading between 20.0 and 30.0
  return random(200, 300) / 10.0;
}

// Record last sensor values input on NVS
// Keep last 5 sensor values
void RandomSensorController::saveLastSensorValue(float value) {
  nvs_handle_t storageHandle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &storageHandle);

  if (err != ESP_OK) {
    Serial.printf("NVS open error: %s\n", esp_err_to_name(err));
    return;
  }

  int32_t value1 = 0;
  int32_t value2 = 0;
  int32_t value3 = 0;
  int32_t value4 = 0;

  int32_t newValue = (int32_t)(value * 10);
  nvs_get_i32(storageHandle, NVS_KEY_1, &value1);
  nvs_get_i32(storageHandle, NVS_KEY_2, &value2);
  nvs_get_i32(storageHandle, NVS_KEY_3, &value3);
  nvs_get_i32(storageHandle, NVS_KEY_4, &value4);

  // Reorganizes storage to keep the last 5 values
  err = nvs_set_i32(storageHandle, NVS_KEY_1, newValue);
  err = nvs_set_i32(storageHandle, NVS_KEY_2, value1);
  err = nvs_set_i32(storageHandle, NVS_KEY_3, value2);
  err = nvs_set_i32(storageHandle, NVS_KEY_4, value3);
  err = nvs_set_i32(storageHandle, NVS_KEY_5, value4);

  if (err == ESP_OK) {
    // Commit the changes to save them in NVS
    err = nvs_commit(storageHandle);
    if (err == ESP_OK) {
      Serial.printf("Value %.2f recorded.\n", value);
    }
  }

  nvs_close(storageHandle);
}

unsigned long RandomSensorController::getRefreshInterval() {
  nvs_handle_t storageHandle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &storageHandle);
  
  if (err != ESP_OK) {
    Serial.printf("NVS Open Error: %s\n", esp_err_to_name(err));
    
    nvs_close(storageHandle); 
    return refrehInterval;
  }
  
  uint32_t storedValue = refrehInterval;
  nvs_get_u32(storageHandle, NVS_KEY_REFRESH_INTERVAL, &storedValue);

  if (err != ESP_OK) {
    Serial.printf("NVS Read Error: %s\n", esp_err_to_name(err));

    nvs_close(storageHandle); 
    return refrehInterval;
  }

  nvs_close(storageHandle); 
  
  return (unsigned long)storedValue;
}

void RandomSensorController::setRefreshInterval(unsigned long value) {  
  nvs_handle_t storageHandle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &storageHandle);

  if (err != ESP_OK) {
    Serial.printf("NVS Open error: %s\n", esp_err_to_name(err));
    return;
  }

  err = nvs_set_u32(storageHandle, NVS_KEY_REFRESH_INTERVAL, (uint32_t)value);

  if (err == ESP_OK) {
    // Commit the changes to save them in NVS
    err = nvs_commit(storageHandle);
    if (err == ESP_OK) {
      refrehInterval = value;
      Serial.printf("New refresh interval recorded : %s\n", String(refrehInterval));
    } else {
      Serial.printf("NVS Error %s\n", esp_err_to_name(err));
    }
  } else {
    Serial.printf("NVS Error %s\n", esp_err_to_name(err));
  }

  nvs_close(storageHandle);
}

std::array<float, 5> RandomSensorController::getSensorValues() {
  int32_t value1 = 0;
  int32_t value2 = 0;
  int32_t value3 = 0;
  int32_t value4 = 0;
  int32_t value5 = 0;
  
  std::array<float, 5> temperatures;

  nvs_handle_t storageHandle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &storageHandle);
  
  if (err != ESP_OK) {
    Serial.printf("NVS Open Error: %s\n", esp_err_to_name(err));
    return temperatures;
  }

  nvs_get_i32(storageHandle, NVS_KEY_1, &value1);
  nvs_get_i32(storageHandle, NVS_KEY_2, &value2);
  nvs_get_i32(storageHandle, NVS_KEY_3, &value3);
  nvs_get_i32(storageHandle, NVS_KEY_4, &value4);
  nvs_get_i32(storageHandle, NVS_KEY_5, &value5);
  
  nvs_close(storageHandle);

  temperatures[0] = float(value1) / 10;
  temperatures[1] = float(value2) / 10;
  temperatures[2] = float(value3) / 10;
  temperatures[3] = float(value4) / 10;
  temperatures[4] = float(value5) / 10;
 
  return temperatures; 
}

void RandomSensorController::begin() {
  refrehInterval = getRefreshInterval();  
}

void RandomSensorController::readSensorLoop() {
  unsigned long now = millis();

  // Check if the time since the last action is >= the playback interval
  if (now - _lastActionTime >= refrehInterval) {
    _lastActionTime = now;

    // Reads sensor data
    float temperature = readSensorValue();
    
    // Records last value
    saveLastSensorValue(temperature);
  }
}