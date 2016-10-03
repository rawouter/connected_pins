#include <stdarg.h>

// Some platforms do not support Serial.printf
void serprintf(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
}

void setup_serial() {
  //while (!Serial);  // required for Flora & Micro, but hangs the ESP8266
  delay(500);

  Serial.begin(115200);
  pinMode(0, OUTPUT); digitalWrite(0, HIGH);
  pinMode(2, OUTPUT); digitalWrite(2, HIGH);
  Serial.println("Alive");
}

void set_color_from_char(char c) {
  switch (c) {
    case 'E': set_color(255, 50, 180); break;
    case 'a': set_color(255, 0, 0); break;
    case 'c': set_color(255, 40, 40); break;
    case 'e': set_color(255, 85, 30); break;
    case 'w': set_color(255, 255, 0); break;
    case 'n': set_color(15, 100, 255); break;
    case 'i': set_color(40, 255, 70); break;
    case 'd': set_color(100, 255, 100); break;
    case 'o': set_color(100, 100, 100); break;
    default:
      Serial.println("set_color_from_char: unknown char");
      break;
  }
}
