#include <Arduino.h>

// Where to get color commands from
// SERIAL can be enabled together with BLE or WIFI
#define MODE_BLE
#define MODE_SERIAL

#define DEBUG

void setup() {
  setup_serial();
  
  #ifdef MODE_BLE
  setup_ble();
  #endif

  setup_neopixel();
}

void loop() {
  
  #ifdef MODE_BLE
  char buf[65];  // size of BLE buffer, for now keep json below 64 chars!
  if (read_string_from_ble(buf)) {
    debug(buf);    
    set_color_from_json(buf);
    display_leds();
  } 
  #endif

  #ifdef MODE_SERIAL
  if (Serial.available() > 0) {
    char c = Serial.read();
    set_color_from_char(c);
    display_leds();
  }
  #endif
  
  delay(100);
}

void debug(char *buf) {
  #ifdef DEBUG
  Serial.println(buf);
  #endif
}







