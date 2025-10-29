#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "output.h"

const char* serverHost = "192.168.1.50"; // <--- แก้ไข
const int serverPort = 3000;
const char* apiPath = "/record/create";
const char* controlPath = "/control/relay/32"; 

// ตั้งค่าเวลาในการส่งข้อมูล (ทุก 5 min)
const long postInterval = 300000;
unsigned long previousMillis = 0;

void sendDataToFlask(float temp, float humidity, float lux, float Soil) {
  // สร้าง JSON Object ที่จะส่งไป
  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["lux"] = lux;
  doc["soil_humid"] = Soil;
 

  String jsonPayload;
  serializeJson(doc, jsonPayload);

  Serial.print("Attempting to POST: ");
  Serial.println(jsonPayload);

  HTTPClient http;
  
  // สร้าง URL เต็ม: http://192.168.1.50:3000/record/create
  String serverUrl = "http://" + String(serverHost) + ":" + String(serverPort) + String(apiPath);
  http.begin(serverUrl);
  
  // กําหนด Header ว่าเราส่งข้อมูลในรูปแบบ JSON
  http.addHeader("Content-Type", "application/json");

  // ส่งข้อมูล POST
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.print("HTTP POST Error Code: ");
    Serial.println(httpResponseCode);
    Serial.println(http.errorToString(httpResponseCode));
  }

  http.end(); // ปิดการเชื่อมต่อ
}


void checkRelayStatus() {
  HTTPClient http;
  
  String serverUrl = "http://" + String(serverHost) + ":" + String(serverPort) + String(controlPath);
  http.begin(serverUrl);

  Serial.print("Checking relay status from: ");
  Serial.println(serverUrl);

  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("Received status: ");
    Serial.println(payload);

    // ใช้ ArduinoJson ในการ Parse JSON Response
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    String status = doc["status"].as<String>();
    
    // 2. สั่งการ GPIO ตามสถานะที่ได้รับจาก Server
    if (status == "ON") {
      OnPump();
      Serial.println("-> PUMP SET TO HIGH (ON)");
    } else {
      digitalWrite(Pump, LOW);
      Serial.println("-> GPIO SET TO LOW (OFF)");
    }

  } else {
    Serial.print("HTTP GET Error Code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}