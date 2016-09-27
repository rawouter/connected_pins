#include <Adafruit_NeoPixel.h>
#include <math.h>

#define MODE_RAINBOW
#define MODE_SERIAL
#define PIN 12
#define BREATH_TIME 3000 // MilliSecond

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
unsigned long start_color_time = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  set_color(strip.Color(0, 0, 255));
  strip.show(); // Initialize all pixels to 'off'

#ifdef MODE_RAINBOW
  rainbow_demo();
#endif
}

void loop() {
#ifdef MODE_SERIAL
    set_color_from_serial();
#endif

  // Redraw pixel with brightness
  float bright = get_brightness();
  strip.setBrightness(bright);
  strip.show();
}

void set_color(uint32_t c) {
  Serial.print("Setting color: ");
  Serial.println(c);
  strip.setBrightness(255);
  start_color_time = millis();
  strip.setPixelColor(0, c);
}

float get_brightness() {
  unsigned long now = millis() - start_color_time;
  if (now > BREATH_TIME) {
    return 0;
  } else {
    Serial.println(now);
    return (exp(sin(now/2000.0*PI)) - 0.36787944)*108.0;
  }
}

#ifdef MODE_SERIAL
void set_color_from_serial() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'o': set_color(strip.Color(0,0,0)); break;
      case 'r': set_color(strip.Color(255,0,0)); break;
      case 'g': set_color(strip.Color(0,255,0)); break;
      case 'b': set_color(strip.Color(0,0,255)); break;
      case 'y': set_color(strip.Color(255,255,0)); break;
      case 'm': set_color(strip.Color(255,0,255)); break;
      case 'c': set_color(strip.Color(0,255,255)); break;
      case 'w': set_color(strip.Color(255,255,255)); break;
    }
  }
}
#endif

#ifdef MODE_RAINBOW
void rainbow_demo() {
  const int DELAY = 2000;
  set_color(strip.Color(255,51,153)); // emergency
  strip.show();
  delay(DELAY);
  set_color(strip.Color(255,0,0)); // alert
  strip.show();
  delay(DELAY);
  set_color(strip.Color(255,80,80)); // critical
  strip.show();
  delay(DELAY);
  set_color(strip.Color(255,153,51)); // error
  strip.show();
  delay(DELAY);
  set_color(strip.Color(255,255,0)); // warning
  strip.show();
  delay(DELAY);
  set_color(strip.Color(51,153,255)); // notice
  strip.show();
  delay(DELAY);
  set_color(strip.Color(111,252,182)); // info
  strip.show();
  delay(DELAY);
  set_color(strip.Color(162,250,150)); // debug
  strip.show();
  delay(500);
}
#endif
