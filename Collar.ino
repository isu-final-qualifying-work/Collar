#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
// float totalvec = 0.0;
// float x, y, z;
// int steps = 0;
// #define WIFI_SSID "111"
// #define WIFI_PASS "11111111"

void setup(){
  Serial.begin(9600); 
  Serial.println("Starting BLE work!");

  BLEDevice::init("OSHEYNIK_001");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic =
    pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  // if(!accel.begin()){
  //     while(1);
  // }
  // WiFi.begin(WIFI_SSID, WIFI_PASS);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
}

void loop(){
  // HTTPClient http;
  // WiFiClient client;
  // sensors_event_t event;
  // accel.getEvent(&event);
  // Serial.print("Steps = "); 
  // Serial.println(steps); 
  // x = event.acceleration.x;
  // y = event.acceleration.y;
  // z = event.acceleration.z;
  // Serial.print("X = "); 
  // Serial.print(x); 
  // Serial.print(";  Y = "); 
  // Serial.print(y); 
  // Serial.print(";  Z = "); 
  // Serial.print(z); 
  // Serial.println("  m/s^2 ");
  // float vec = sqrt(x * x + y * y + z * z);
  // totalvec = abs(vec - totalvec);
  // Serial.print("V = "); 
  // Serial.println(totalvec); 
  // if (totalvec > 1.5){
  //   steps++;
  
  // totalvec = vec;
  // if (http.begin(client, "http://172.20.10.2:8000/add_activity")){
  //   http.addHeader("accept", "application/json");
  //   http.addHeader("Content-Type", "application/json");
  //   StaticJsonDocument<200> TempDataJSON;
  //   TempDataJSON["collar"] = WiFi.macAddress();
  //   TempDataJSON["x"] = x;
  //   TempDataJSON["y"] = y;
  //   TempDataJSON["z"] = z;
  //   String TempDataString;
  //   serializeJson(TempDataJSON, TempDataString);
  //   int httpResponseCode = http.POST(TempDataString);
  //   Serial.print("Server responce = "); 
  //   Serial.println(httpResponseCode); 
  //   }
  // }
}