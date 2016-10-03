/////////
// WIFI
/////////
#ifdef USE_WIFI

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Hash.h>

#include <DNSServer.h>          //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>   //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>        //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

// Configure your settings here:
#include "user_settings.h"

char mqtt_server[40] = AIO_SERVER;
char mqtt_port[6] = "8883";

void set_wifi_led() {
  // blue led when wifi is on
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(2, LOW);
  }
  digitalWrite(2, HIGH);
}

void setup_wifi() {
  // blue led shows wifi status
  pinMode(2, OUTPUT); digitalWrite(2, HIGH);

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
  WiFiManager wifiManager;
  //wifiManager.setSaveConfigCallback(saveConfigCallback);

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);

  //reset settings - for testing
  //wifiManager.resetSettings();

  if (!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  Serial.print("MQTT Server: "); Serial.println(mqtt_server);
  Serial.print("MQTT Port: "); Serial.println(mqtt_port);
}

#endif


