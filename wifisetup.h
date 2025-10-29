#include <WiFiManager.h>
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
}