#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <math.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>

//#define DEBUG
#define MODE_SERIAL
#define MODE_CONNECTED

#define SSID "xxxx"
#define SSIDPASS "xxxx"
#define SSOCKETURL "shrouded-bayou-62366.herokuapp.com"

#define FREQ 1             // Number of blink(s) during BLINK_TIME
#define BLINK_TIME 2       // Total ligthning time per color message (seconds)
#define MAXBRIGHTNESS 255  // 0 to 255, overall brigthness of the leds
#define PIN 12             // Pin number of the NeoPixel bus
#define NUM_PIX 1          // Number of NoePixel leds

#ifdef MODE_CONNECTED
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRB + NEO_KHZ800);
uint8_t current_color[] = {0, 0, 255};
unsigned long start_color_time = 0;

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT); digitalWrite(0, HIGH);
  pinMode(2, OUTPUT); digitalWrite(2, HIGH);

  #ifdef MODE_CONNECTED
  #ifdef DEBUG
  Serial.setDebugOutput(true);  // Will print WIFI debugs
  Serial.println();
  #endif

  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(SSID, SSIDPASS);

  //WiFi.disconnect();
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  webSocket.beginSSL(SSOCKETURL, 443);
  webSocket.onEvent(webSocketEvent);
  #endif

  strip.begin();
  strip.setBrightness(MAXBRIGHTNESS);
  set_color(current_color);
  strip.show(); // Initial blip
}

void loop() {
  #ifdef MODE_SERIAL
  set_color_from_serial();
  #endif

  #ifdef MODE_CONNECTED
  webSocket.loop();
  #endif

  // Connection check
  // Red led = WebSocket connected
  //if (webSocket.clientIsConnected(*webSocket)==true) {
  //  digitalWrite(0, LOW);
  //}
  //digitalWrite(0, HIGH);

  // Blue led = WIFI connected
  #ifdef MODE_CONNECTED
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(2, LOW);
  }
  digitalWrite(2, HIGH);
  #endif

  // Redraw pixels
  draw_pixels();
  strip.show();
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t c[] = {r,g,b};
  set_color(c);
}

/*
 * Record the start time and the new color to reach
 */
void set_color(uint8_t c[3]) {
  #ifdef DEBUG
  Serial.printf("Setting color: %i,%i,%i\n", c[0], c[1], c[2]);    
  #endif

  start_color_time = millis();
  current_color[0] = c[0];
  current_color[1] = c[1];
  current_color[2] = c[2];
}

void draw_pixels() {
  uint8_t c[3] = {0,0,0};
  uint8_t brightness = (uint8_t)get_brightness();

  c[0] = map(current_color[0], 0, 255, 0, brightness);
  c[1] = map(current_color[1], 0, 255, 0, brightness);
  c[2] = map(current_color[2], 0, 255, 0, brightness);

  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c[0], c[1], c[2]);
  }
}

uint8_t get_brightness() {
  uint8_t brightness;
  float now = (millis() - start_color_time) / 1000.0; // Seconds
  if (now > BLINK_TIME) {
    digitalWrite(0, HIGH);
    brightness = 0;
  } else {
    digitalWrite(0, HIGH*0.8);
    //brightness = (uint8_t)abs(sin(now*PI/BLINK_TIME*FREQ) * 255);
    brightness = (uint8_t)((exp(sin((now*2*PI/BLINK_TIME*FREQ)-PI/2.0)) - 0.36787944)*108.0);
  }
  return brightness;
}

#ifdef MODE_SERIAL
void set_color_from_serial() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'E': set_color(255,50,180); break;
      case 'a': set_color(255,0,0); break;
      case 'c': set_color(255,40,40); break;
      case 'e': set_color(255,85,30); break;
      case 'w': set_color(255,255,0); break;
      case 'n': set_color(15,100,255); break;
      case 'i': set_color(40,255,70); break;
      case 'd': set_color(100,255,100); break;
      case 'o': set_color(100,100,100); break;
    }
  }
}
#endif

#ifdef MODE_CONNECTED
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    String str;
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                Serial.printf("[WSc] Connected to url: %s\n",  payload);
                webSocket.sendTXT("Connected"); // Say hello to server when connected
            }
            break;
        case WStype_TEXT:
            #ifdef DEBUG
            Serial.printf("[WSc] get text: %s\n", payload);
            #endif
            str = (char*)payload;
            // TODO: This can be surely be done better, like, get the first key looking at double quotes.
            if (str.substring(2,7).equals("color")) {
              uint8_t pos1, pos2;
              uint8_t r, g, b;
              pos1 = str.indexOf('[');
              pos2 = str.indexOf(',');
              r = str.substring(pos1+1, pos2).toInt();
              pos1 = pos2;
              pos2 = str.indexOf(',', pos1+1);
              g = str.substring(pos1+1, pos2).toInt();
              pos1 = pos2;
              pos2 = str.indexOf(']', pos1+1);
              b = str.substring(pos1+1, pos2).toInt();
              set_color(r,g,b);
            }
            break;
        case WStype_BIN:
            #ifdef DEBUG
            Serial.printf("[WSc] get binary length: %u\n", length);
            #endif
            hexdump(payload, length);
            break;
    }
}
#endif
