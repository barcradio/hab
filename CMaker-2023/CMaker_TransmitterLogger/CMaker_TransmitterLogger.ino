// CMaker_TransmitterLogger.ino
//
// High Altitude Balloon class
// Cache Makers and Bridgerland Amateur Radio Club
//
// To compile this code for the ground stations ...
//
// 1. Make sure required libraries are in ~/arduino/libraries
//        libraries/ArdusatSDK-master
//        libraries/DallasTemperature
//        libraries/OneWire
//        libraries/RadioHead
//        libraries/TinyGPSPlus
// 0. Make sure required libraries are in ~/arduino/libraries
// 2. in Arduino IDE, select Tools->Board: "Moteino AVR Boards" -> "MoteinoMEGA(-USB)"
// 3. in Arduino IDE, select Tools->Port: <COM port for transmitter>
// 4. compile and upload
//


#include "lora.h"; // RH_RF95 LoRa board on Moteino Mega R4
#include "status.h" // RGB Status LED (optional)
#include "temp.h"; // DS18B20 temperature sensors
#include "batt.h"; // battery voltage monitoring
#include "buzzer.h"; // piezo buzzer
#include "gps.h"; // GM401R GPS receiver
#include "spaceBoard.h"; // space sensor board from Because Learning
#include "logger.h"; // onboard data logging to micro SD card
#include "telemetry.h";

void clearTelemetry() {
  memset(&telemetry, 0, sizeof(telemetry));
  telemetry.id = CACHE_MAKERS_ID;
  telemetry.packetType = 0x01;
}

void setup(void)
{
  Serial.begin(115200);
  initTemp();
  initBatt();
  initBuzzer();
  initGPS();
  initLoRa();
  initLogger();
  initSpaceBoard();
  
  // Send initial header line to the logger
  writeLogHeader();
}


long lastTransmission = 0;
long telemetryInterval = 10000; // transmit and log every ten seconds

/*
   Main loop.
*/
void loop(void)
{
//  testLoop();
  long t = millis();
  if (t - lastTransmission >= telemetryInterval) {
    Serial.println("Sending Telemetry...");
    lastTransmission = t;
    sendTelemetry();
    beep();
  }
  delay(100);
}

void sendTelemetry() {
  readGPS();
  testGPS();
  
  // read all sensors - updates sensor objects with new values
  readSensors();
  
  // fill Telemetry struct...
  clearTelemetry();
  if (gps.location.isValid()) {
    telemetry.gpslat = gps.location.lat();
    telemetry.gpslon = gps.location.lng();
    telemetry.gpsalt = gps.altitude.meters();
    telemetry.gpsspeed = gps.speed.kmph();
  } else {
    // FOR TESTING ONLY - REMOVE WHEN GPS IS WORKING!
    telemetry.gpslat = 42.345;
    telemetry.gpslon = -111.123;
    telemetry.gpsalt = 123456.789 / 3.28084;
    telemetry.gpsspeed = 123.456 / 0.621371;
  }
  telemetry.temperature = temp.t;
  telemetry.luminosity = lum.lux;
  telemetry.uvlight = uv.uvindex;
  telemetry.vBatt = readBatt();
  readTemperatures();
  telemetry.tempBatt = tempBattery;
  telemetry.tempInt = tempInterior;
  telemetry.tempExt = tempExterior;

  telemetry.checksum = calculateTelemetryChecksum(&telemetry);
  
  // send LoRa packet
  sendLoRa((uint8_t *)&telemetry, sizeof(Telemetry));
  Serial.print("sent ");
  Serial.print(sizeof(Telemetry));
  Serial.println(" bytes");
  
  // log all data to onboard logger
  writeLogEntry();
}

void testLoop() {
  Serial.println("Starting Test Loop...");
  testTemp();
  testBatt();
  //testBuzzer();
  //testSpaceBoard();
  testGPS();
  
  Serial.println();
  delay(1000);
}
