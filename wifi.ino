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

//for LED status
#include <Ticker.h>
Ticker wifiticker;

#define WIFI_LED 2

char mqtt_server[40] = AIO_SERVER;
char mqtt_port[6] = "8883";

void tick() {
  int state = digitalRead(WIFI_LED);  // get the current state of GPIO1 pin
  digitalWrite(WIFI_LED, !state);     // set pin to the opposite state
}

void set_wifi_led() {
  // blue led when wifi is on
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(WIFI_LED, LOW);
  }
  digitalWrite(WIFI_LED, HIGH);
}

void setup_wifi() {
  pinMode(WIFI_LED, OUTPUT);
  // Start at 0.6 because we start in AP mode and try to connect
  wifiticker.attach(0.6, tick);


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
  wifiticker.detach();
}

#endif


