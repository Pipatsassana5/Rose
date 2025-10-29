#include "esp32-hal-gpio.h"

#define Pump 32
#define Light 33
void OnPump() {
  digitalWrite(Pump, HIGH);
  delay(2000);
  digitalWrite(Pump, LOW);
}

void OnLight(){
  digitalWrite(Light, HIGH);

}

void OffLight() {
  digitalWrite(Light, LOW);
  
}
