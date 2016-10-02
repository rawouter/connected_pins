////////////////////////////////
// NeoPixel
////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <math.h>

#define FREQ 1             // Number of blink(s) during BLINK_TIME
#define BLINK_TIME 1       // Total ligthning time per color message (seconds)
#define MAXBRIGHTNESS 255  // 0 to 255, overall brigthness of the leds
#define PIN 12             // Pin number of the NeoPixel bus
#define NUM_PIX 1          // Number of NoePixel leds

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRB + NEO_KHZ800);
uint8_t current_color[] = {0, 0, 255};
unsigned long start_color_time = 0;

void setup_neopixel() {
  strip.begin();
  strip.setBrightness(MAXBRIGHTNESS);
  set_color(current_color);
  strip.show(); // Initial blip
}

void display_leds() {
  draw_pixels();
  strip.show();
}

void set_color(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t c[] = {r, g, b};
  set_color(c);
}

/*
   Record the start time and the new color to reach
*/
void set_color(uint8_t c[3]) {
#ifdef DEBUG
  serprintf("Setting color: %i,%i,%i\n", c[0], c[1], c[2]);
#endif

  start_color_time = millis();
  current_color[0] = c[0];
  current_color[1] = c[1];
  current_color[2] = c[2];
}

void draw_pixels() {
  uint8_t c[3] = {0, 0, 0};
  uint8_t brightness = (uint8_t)get_brightness();

  c[0] = map(current_color[0], 0, 255, 0, brightness);
  c[1] = map(current_color[1], 0, 255, 0, brightness);
  c[2] = map(current_color[2], 0, 255, 0, brightness);

#ifdef DEBUG
  if (brightness > 0) {
    serprintf("Brightness: %i\n", brightness);
    serprintf("Resulting color: %i,%i,%i\n", c[0], c[1], c[2]);
  }
#endif

  for (uint16_t i = 0; i < strip.numPixels(); i++) {
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
    digitalWrite(0, HIGH * 0.8);
    //brightness = (uint8_t)abs(sin(now*PI/BLINK_TIME*FREQ) * 255);
    brightness = (uint8_t)((exp(sin((now * 2 * PI / BLINK_TIME * FREQ) - PI / 2.0)) - 0.36787944) * 108.0);
  }
  return brightness;
}


