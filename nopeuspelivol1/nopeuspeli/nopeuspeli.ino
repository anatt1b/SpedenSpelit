#include <Arduino.h>
#include "leds.vol.h"
#include "buttons.vol.h"




void setup() {
Serial.begin(9600);// put your setup code here, to run once:
initializeLeds();
initButtonsAndButtonInterrupts();


}

void loop() {


show1();  

delay (2000);
  // put your main code here, to run repeatedly:
show2(4);

delay(100);


}