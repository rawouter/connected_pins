#include <Arduino.h>

// If using WIFI/MQTT copy user_settings.h.template to user_settings.h and adjust it

// Where to get color commands from
// SERIAL can be enabled together with BLE or WIFI
// WIFI can be used with MQTT or WEBSOCKET (todo)

//#define USE_BLE
#define USE_WIFI
#define USE_MQTT
#define USE_SERIAL

#define DEBUG

#define BUF_SIZE 65
char buf[BUF_SIZE];  // size of BLE buffer, for now keep json smaller!

void setup() {
  setup_serial();

#ifdef USE_WIFI
  setup_wifi();
#endif

#ifdef USE_MQTT
  setup_mqtt();
#endif

#ifdef USE_BLE
  setup_ble();
#endif

  setup_neopixel();
}

void loop() {

#ifdef USE_MQTT
  MQTT_connect();
  if (read_string_from_mqtt(buf, BUF_SIZE)) {
    debug(buf);
    set_color_from_json(buf);
  }
  //mqtt_keepalive();
#endif

#ifdef USE_BLE
  if (read_string_from_ble(buf, BUF_SIZE)) {
    debug(buf);
    set_color_from_json(buf);
  }
#endif

#ifdef USE_SERIAL
  if (Serial.available() > 0) {
    char c = Serial.read();
    set_color_from_char(c);
  }
#endif

  display_leds();
}

void debug(char *buf) {
#ifdef DEBUG
  Serial.println(buf);
#endif
}







