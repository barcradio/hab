//
// temp.h - read ds18b20 temperature sensors
//

#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_TEMP 19

float tempInterior; // T0
float tempBattery;  // T1
float tempExterior; // T2

DeviceAddress T0 = { 0x28, 0x17, 0x82, 0x80, 0xE3, 0xE1, 0x3C, 0xDE }; // T0 Onboard
DeviceAddress T1 = { 0x28, 0x67, 0x08, 0x80, 0xE3, 0xE1, 0x3C, 0x30 }; // T1 Battery
DeviceAddress T2 = { 0x28, 0xD8, 0x03, 0x80, 0xE3, 0xE1, 0x3C, 0x63 }; // T2 Exterior
DeviceAddress T3 = { 0x28, 0x04, 0x98, 0x80, 0xE3, 0xE1, 0x3C, 0x1D }; // T3 ----


// DS18B20 Temperature Sensors
#define ONE_WIRE_BUS PIN_TEMP
#define TEMPERATURE_PRECISION 9

// Temperature Sensors
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);

void initTemp() {
  Serial.println("Initializing DS18B20 Temperature sensors...");
  tempSensors.begin();

  // locate devices on the bus
  Serial.print("Found ");
  Serial.print(tempSensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  
  // report parasite power requirements
  Serial.print("Parasite power is: ");
  Serial.println(tempSensors.isParasitePowerMode() ? "ON" : "OFF");

  // set to 9 bit resolution
  tempSensors.setResolution(T0, TEMPERATURE_PRECISION);
  tempSensors.setResolution(T1, TEMPERATURE_PRECISION);
  tempSensors.setResolution(T2, TEMPERATURE_PRECISION);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

float readTemp(DeviceAddress deviceAddress) {
  float tempC = tempSensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
    return 0;
  return tempC;
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = tempSensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  float tempF = tempC * 9 / 5 + 32;
  Serial.print(tempF);
  Serial.print(" F");
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(tempSensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("DS18B20 Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void testTemp(void)
{
  // call tempSensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  tempSensors.requestTemperatures();

  // print the device information
  printData(T0);
  //printData(T1);
  //printData(T2);
  //printData(T3);
}

void readTemperatures() {
  tempSensors.requestTemperatures();
  tempInterior = tempSensors.getTempC(T0);
  tempBattery  = tempSensors.getTempC(T1);
  tempExterior = tempSensors.getTempC(T2);
}
