#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
// float totalvec = 0.0;
float x, y, z;
// int steps = 0;
#define WIFI_SSID "111"
#define WIFI_PASS "11111111"

void setup(){
  Serial.begin(9600); 
  if(!accel.begin()){
      Serial.println("No ADXL345 sensor detected.");
      while(1);
  }
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
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
  // float vec = sqrt(x * x + y * y + z * z);
  // totalvec = abs(vec - totalvec);
  // Serial.print("V = "); 
  // Serial.println(totalvec); 
  // if (totalvec > 1.5){
  //   steps++;
  // }
  // totalvec = vec;
  if (http.begin(client, "http://172.20.10.2:8000/add_activity")){
    http.addHeader("accept", "application/json");
    http.addHeader("Content-Type", "application/json");
    StaticJsonDocument<200> TempDataJSON;
    TempDataJSON["collar"] = WiFi.macAddress();
    TempDataJSON["x"] = x;
    TempDataJSON["y"] = y;
    TempDataJSON["z"] = z;
    String TempDataString;
    serializeJson(TempDataJSON, TempDataString);
    int httpResponseCode = http.POST(TempDataString);
    Serial.print("Server responce = "); 
    Serial.println(httpResponseCode); 
  }
  delay(1000);
}