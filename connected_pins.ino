#include <Arduino.h>

// If using WIFI/MQTT copy user_settings.h.template to user_settings.h and adjust it

// Where to get color commands from
// SERIAL can be enabled together with BLE or WIFI (depending on platform)
// WIFI can be used with MQTT or WEBSOCKET (not both)

#define USE_BLE
//#define USE_WIFI
//#define USE_MQTT
//#define USE_WEBSOCKET
#define USE_SERIAL
//#define USE_RGB
#define USE_RGBW

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

#ifdef USE_WEBSOCKET
  setup_websocket();
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
    set_color_from_json(buf);
  }
#endif

#ifdef USE_BLE
  if (read_string_from_ble(buf, BUF_SIZE)) {
    set_color_from_json(buf);
  }
#endif

#ifdef USE_WEBSOCKET
  check_websocket();
#endif

#ifdef USE_SERIAL
  if (Serial.available() > 0) {
    char c = Serial.read();
    set_color_from_char(c);
  }
#endif

#ifdef USE_WIFI
  set_wifi_led();
#endif

  display_leds();
}

void debug(char *buf) {
#ifdef DEBUG
  Serial.println(buf);
#endif
}







