//
// batt.h
// Read analog battery voltage from voltage divider on 7.4v Li-ion input.
//
#define BATT_PIN A0
#define DIV_RATIO 2.963 // voltage divider ratio at A0 (orig 2.98)
float batteryVoltage = 0.0;

float lowBatteryLevel = 7.2;

float readBatt() {
  int scaled = analogRead(BATT_PIN);
  batteryVoltage = scaled * 3.3 * DIV_RATIO / 1024; // 3.3v reference
  // Q. Do we want to do anything if voltage gets too low?
  return batteryVoltage;
}

void initBatt() {
  Serial.println("Initializing Battery monitor...");
  if (readBatt() < lowBatteryLevel)
    setPayloadError(LOW_BATTERY);
}

testBatt() {
  Serial.print("Battery Voltage: ");
  Serial.print(readBatt());
  Serial.println(" V");
}
