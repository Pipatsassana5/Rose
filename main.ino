#include <Wire.h>
#include <BH1750.h>
#include <SHT21.h>
#include "wifisetup.h"
#include "output.h"
#include "HTTP.h"
// === การกำหนดค่า I2C สำหรับ Light Sensor (BH1750) ===
#define LIGHT_SDA_PIN 18 
#define LIGHT_SCL_PIN 19 
#define Soil_Moisture  36
// สร้างอ็อบเจ็กต์ TwoWire ใหม่ สำหรับ I2C Controller 1
TwoWire I2C_Light = TwoWire(1); 
// ===============================================
BH1750 lightMeter;
SHT21 sht; // ใช้ขา original คือ 21,22

void setup() {
  pinMode(Pump, OUTPUT);
  pinMode(Light, OUTPUT);
  Serial.begin(115200); // แนะนำให้ใช้ Baud Rate ที่สูงขึ้น เช่น 115200
  Wifisetup();
  Wire.begin(); 
  Serial.println("SHT21 I2C Initialized on Default Pins (SDA: 21, SCL: 22)");

  // กำหนดขาใหม่ตามโจทย์: SDA=18, SCL=19
  I2C_Light.begin(LIGHT_SDA_PIN, LIGHT_SCL_PIN);
  Serial.print("BH1750 I2C Initialized on Pins: SDA=");
  Serial.print(LIGHT_SDA_PIN);
  Serial.print(", SCL=");
  Serial.println(LIGHT_SCL_PIN);

  // ผูกอ็อบเจ็กต์ BH1750 เข้ากับบัส I2C ที่กำหนด (I2C_Light)
  // *สำคัญ:* ต้องระบุ TwoWire object เข้าไปในการเริ่มต้น
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &I2C_Light)) {
    Serial.println("BH1750 Light Sensor Ready"); 
  } else {
    Serial.println("Error: BH1750 not found on I2C Controller 1!");
  }

  Serial.println(F("Initialization Complete."));
}

void loop() {
  // === อ่านค่าจาก Light Sensor (BH1750) บน I2C Controller 1 ===
  checkRelayStatus();
  float lux = lightMeter.readLightLevel(); 
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  // === อ่านค่าจาก Temp/Humidity Sensor (SHT21) บน I2C Controller 0 ===
  float temp = sht.getTemperature(); 
  float humidity = sht.getHumidity();

  Serial.print("อุณหภูมิ: ");
  Serial.print(temp);
  Serial.print(" °C");
  Serial.print("  ความชื้น: ");
  Serial.print(humidity);
  Serial.println(" %");

  float Soil = analogRead(Soil_Moisture);
  Serial.print("ดิน = " );
  Serial.println(Soil);
  // หน่วงเวลา
  // SHT21 ต้องการขั้นต่ำ 85ms สำหรับการอ่านค่า 14-bit [cite: 9]
  // เราจะใช้ 1000ms เพื่อให้ข้อมูลไม่ล้น Serial Monitor
  delay(1000);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= postInterval) {
    previousMillis = currentMillis;
    
    // ตรวจสอบว่ายังเชื่อมต่อ Wi-Fi อยู่
    if (WiFi.status() == WL_CONNECTED) {

      sendDataToFlask(temp, humidity, lux, Soil); // ส่งข้อมูล
    } else {
      Serial.println("WiFi disconnected. Reconnecting...");
      WiFi.reconnect();
    }
  }

}