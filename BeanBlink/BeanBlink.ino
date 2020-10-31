/* 
  This sketch uses the Bean library to blink the on-board RGB LED. 
  
  Notes:
    - This is not a low-power sketch 
    - A Bean with a low battery might show a faint blue and green LED color
  
  This example code is in the public domain.
*/

void setup() {
}

void setLedColorsAndSleep(int r, int g, int b, int sleep) {
  Bean.setLed(r, g, b);
  Bean.sleep(sleep);   
}

void loop() {
  // Turn the Bean's LED red
  setLedColorsAndSleep(255, 0, 0, 1000);
  // Turn the Bean's LED green  
  setLedColorsAndSleep(0, 255, 0, 1000);
  // Turn the Bean's LED blue
  setLedColorsAndSleep(0, 0, 255, 3000);
  // Turn off the Bean's LED
  setLedColorsAndSleep(255, 255, 255, 5000);
}
