/*#include <WiFiManager.h>
void Wifisetup(){
  WiFiManager wm;
       
    bool res;
    res = wm.autoConnect("AutoRose","password");
    
    if(!res) {
        Serial.println("Failed to connect");
        ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}*/

#include <WiFi.h>

const char *ssid = "2.4G_changnoy24";          // Change this to your WiFi SSID
const char *password = "tonkun12345";  // Change this to your WiFi password

void Wifisetup() {
  
  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
