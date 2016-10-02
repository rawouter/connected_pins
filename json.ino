#include <ArduinoJson.h>

void set_color_from_json(char *json) {
  StaticJsonBuffer<200> jsonBuffer;
  
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  int red   = root["color"][0];
  int green = root["color"][1];
  int blue  = root["color"][2];

  set_color(red, green, blue);
}
