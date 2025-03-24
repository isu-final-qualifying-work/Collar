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
#define SSID "****"
#define PASS "********"


Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

float totalvec = 0.0;
float x, y, z;
float old_vec = 0.0;

int steps = 0;

void setup(){
  Serial.begin(9600); 
  Serial.println("Starting BLE work!");

  BLEDevice::init("COLLAR_001");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic =
    pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  if(!accel.begin()){
    Serial.println("Не удалось найти ADXL345. Проверьте подключение.");
      while(1);
  }
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  accel.setRange(ADXL345_RANGE_2_G);
  Serial.println("ADXL345 готов к работе!");
}

void loop(){
  HTTPClient http;
  WiFiClient client;
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.print("Steps = "); 
  Serial.println(steps); 
  x = event.acceleration.x;
  y = event.acceleration.y;
  z = event.acceleration.z;
  Serial.print("X = "); 
  Serial.print(x); 
  Serial.print(";  Y = "); 
  Serial.print(y); 
  Serial.print(";  Z = "); 
  Serial.print(z); 
  Serial.println("  m/s^2 ");
  float vec = sqrt(x * x + y * y + z * z);
  totalvec = (vec + old_vec) / 2;
  Serial.print("V = "); 
  Serial.println(totalvec); 
  if (totalvec > 1){
    steps++;
  if (http.begin(client, "http://192.168.0.103:8000/activity/settings_all")){
    http.addHeader("accept", "application/json");
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<200> TempDataJSON;
    TempDataJSON["collar"] = "COLLAR_001";
    TempDataJSON["x"] = x;
    TempDataJSON["y"] = y;
    TempDataJSON["z"] = z;
    String TempDataString;
    serializeJson(TempDataJSON, TempDataString);
    int httpResponseCode = http.POST(TempDataString);
    Serial.print("Server responce = "); 
    Serial.println(httpResponseCode); 
    }
  }
  old_vec = vec;
}
