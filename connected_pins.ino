#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN 12
#define BREATH_TIME 3000 // MilliSecond

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);
unsigned long start_color_time = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  set_color(strip.Color(0, 0, 255));
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  char c = Serial.read();
  if (c=='r') {set_color(strip.Color(255,0,0));} 
  if (c=='g') {set_color(strip.Color(0,255,0));}
  if (c=='b') {set_color(strip.Color(0,0,255));} 
  if (c=='y') {set_color(strip.Color(255,255,0));}
  if (c=='m') {set_color(strip.Color(255,0,255));} 
  if (c=='c') {set_color(strip.Color(0,255,255));}
  if (c=='w') {set_color(strip.Color(255,255,255));}
  
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
    return (exp(sin(now/2000.0*PI)) - 0.36787944)*108.0;
  }
}
