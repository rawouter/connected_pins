#include <ArduinoJson.h>

void set_color_from_json(char *json) {
  debug(json);

  StaticJsonBuffer<200> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success())
  {
    Serial.println("parseObject() failed");
    return;
  }

  int red   = root["c"][0];
  int green = root["c"][1];
  int blue  = root["c"][2];

  set_color(red, green, blue);
}
