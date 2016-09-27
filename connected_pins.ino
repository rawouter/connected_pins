#include <Adafruit_NeoPixel.h>
#include <math.h>

#define DEBUG
//#define MODE_RAINBOW 1000
#define MODE_SERIAL

#define FREQ 1             // Number of blink(s) during BLINK_TIME
#define BLINK_TIME 2       // Total blinking time
#define MAXBRIGHTNESS 255  // 0 to 255, overall brigthness of the leds
#define PIN 12
#define NUM_PIX 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRB + NEO_KHZ800);
uint8_t current_color[] = {0, 0, 255};
unsigned long start_color_time = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(MAXBRIGHTNESS);
  set_color(current_color);
  strip.show(); // Initial blip
#ifdef MODE_RAINBOW
  rainbow_demo();
#endif
}

void loop() {
  #ifdef MODE_SERIAL
  set_color_from_serial();
  #endif

  // Redraw pixels
  draw_pixels();
  strip.show();
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t c[] = {r,g,b};
  set_color(c);
}

void set_color(uint8_t c[3]) {
  #ifdef DEBUG
  Serial.print("Setting color: ");
  Serial.print(c[0]);
  Serial.print(c[1]);
  Serial.println(c[2]);
  #endif
  
  start_color_time = millis();
  current_color[0] = c[0];
  current_color[1] = c[1];
  current_color[2] = c[2];
}

void draw_pixels() {
  uint8_t c[3] = {0,0,0};
  float brightness = 255 - get_brightness();
  if (brightness>0 && brightness<1) {
    brightness = 1.0;
  }
  if ((0 < brightness) && (brightness < 255)) {
    c[0] = current_color[0] / brightness;
    c[1] = current_color[1] / brightness;
    c[2] = current_color[2] / brightness;

    #ifdef DEBUG
    Serial.print("Brightness (inverted): ");
    Serial.println(brightness);
    Serial.print("Resulting color: ");
    Serial.print(c[0]);
    Serial.print(c[1]);
    Serial.println(c[2]);
    #endif
  }
 
  for(uint16_t i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c[0], c[1], c[2]);
  }
}

float get_brightness() {
  float res;
  unsigned long now = millis() - start_color_time;
  if (now > (BLINK_TIME*1000)) {
    res = 0;
  } else {
    res = sin(now/1000.0*FREQ) * 255;
  }
  return res;
}

#ifdef MODE_SERIAL
void set_color_from_serial() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch(c) {
      case 'o': set_color(0,0,0); break;
      case 'r': set_color(255,0,0); break;
      case 'g': set_color(0,255,0); break;
      case 'b': set_color(0,0,255); break;
      case 'y': set_color(255,255,0); break;
      case 'm': set_color(255,0,255); break;
      case 'c': set_color(0,255,255); break;
      case 'w': set_color(255,255,255); break;
    }
  }
}
#endif

#ifdef MODE_RAINBOW
void rainbow_demo() {
  const int DELAY = MODE_RAINBOW;
  set_color(255,51,153); // emergency
  strip.show();
  delay(DELAY);
  set_color(255,0,0); // alert
  strip.show();
  delay(DELAY);
  set_color(255,80,80); // critical
  strip.show();
  delay(DELAY);
  set_color(255,153,51); // error
  strip.show();
  delay(DELAY);
  set_color(255,255,0); // warning
  strip.show();
  delay(DELAY);
  set_color(51,153,255); // notice
  strip.show();
  delay(DELAY);
  set_color(111,252,182); // info
  strip.show();
  delay(DELAY);
  set_color(162,250,150); // debug
  strip.show();
  delay(500);
}
#endif
