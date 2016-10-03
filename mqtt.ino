/////////
// MQTT
/////////
#ifdef USE_MQTT

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// configure your settings here:
#include "user_settings.h"

const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

//WiFiClient client;
WiFiClientSecure client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe color_feed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME AIO_FEED);

void setup_mqtt() {
  mqtt.subscribe(&color_feed);
}

// if string received, return true and copy the string to the buffer
// if no string received, return false
bool read_string_from_mqtt(char *buf, int size) {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(0))) {
    if (subscription == &color_feed) {
      strncpy(buf, (char *)color_feed.lastread, size - 1);
      buf[size] = '\0';
      return true;
    }
    serprintf("Unknown subscription topic: $s\n", subscription->topic);
    return false;
  }
  return false;
}

void mqtt_keepalive() {
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

#endif
