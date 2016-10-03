// specifics to the ESP8266

#ifdef ESP8266

// set Analog IN to read power voltage
ADC_MODE(ADC_VCC);

// return power voltage in volts
float get_vcc() {
  int32 v = ESP.getVcc();
  float vcc = v / 1000.0;
  return vcc;
}

// return individual chip id as string
int32 get_chip_id() {
  return ESP.getChipId();
}

#endif

