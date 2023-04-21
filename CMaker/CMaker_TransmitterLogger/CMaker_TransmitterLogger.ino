#include <Arduino.h>
#include <Wire.h>
#include <ArdusatSDK.h>
#include <TinyGPS++.h>
#include <RH_RF95.h>

// Radio parameters
//#define RFM95_CS 4
//#define RFM95_RST 3
//#define RFM95_INT 2
//#define RF95_FREQ 915.0  // Transmitter 2 - Red

//RH_RF95 rf95(RFM95_CS, RFM95_INT);

//#define LOG_RX        22 // SoftwareSerial receive (NOT USED - DON'T ASSIGN ELSEWHERE)
//#define LOG_TX        18 // SoftwareSerial transmit to SD Card

//ArdusatSerial logger(SERIAL_MODE_SOFTWARE, LOG_RX, LOG_TX);

// Items to be transmitted...
// 4 bytes per float * 7 values + checksum value = 32 bytes
/*struct Observation {
  float gpslat, gpslon, gpsalt, gpsspeed;
  float temperature;
  float luminosity;
  float uvlight;
  float checksum;
};*/

/*TemperatureTMP temp;
Luminosity lum;
UVLightML uvMl;
TemperatureMLX infrared;
Acceleration accelerometer;
Gyro gyroscope;
Magnetic magnetometer;*/

// GPS Pins
//static const int RXPin = 0, TXPin = 19;
//static const uint32_t GPSBaud = 4800;

// Logging interval in ms. Due to sensor delays, 7300ms = ~10 seconds
//long interval = 7300;
//long previousMillis = 0;

// The TinyGPS++ object
//TinyGPSPlus gps;

// The serial connection to the GPS
//SoftwareSerial ss(RXPin, TXPin);

/*void initPins()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
}

void initSerial()
{
  Serial.begin(9600);
  logger.begin(9600);
  delay(100);

  // Send initial header line to the logger
  logger.println("millis,GPS time,latitude,longitude,altitude,speed,temp,luminosity,UV,infrared,accel.x,accel.y,accel.z,gyro.x,gyro.y,gyro.z,mag.x,mag.y,mag.z");
}

void initRadio()
{
  // Manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    // enter infinite loop until shutdown
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    // enter infinite loop until shutdown
    while (1);
  }
  Serial.print("Set frequency to: ");
  Serial.println(RF95_FREQ);

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}*/

void setup(void)
{  
 /* initPins();
  initSerial();
  initRadio();

  ss.begin(GPSBaud);
  Serial.println(F("Initializing GPS"));

  Serial.println("Initializing sensors");
  accelerometer.begin();
  gyroscope.begin();
  magnetometer.begin();
  temp.begin();
  lum.begin();
  uvMl.begin();
  infrared.begin();
  Serial.println("Sensors OK");*/
}

//float checksum;

//Observation observation;

void loop(void)
{/*
  unsigned long currentMillis = millis();
  
  // If GPS is available, then every interval collect sensor data and transmit 
  while (ss.available() > 0)
    if (gps.encode(ss.read()) && currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;  
      logInfo();
    }*/
}

/*void logInfo()
{
  Serial.println("looping...");

  // Set all observation values to zeros (including checksum)
  // We can do this for floats because Arduino uses IEEE 754
  memset(&observation, 0, sizeof(Observation));

  if (gps.location.isValid()) {
    observation.gpslat = gps.location.lat();
    observation.gpslon = gps.location.lng();
    observation.gpsalt = gps.altitude.meters();
    observation.gpsspeed = gps.speed.kmph();
    observation.checksum += gps.location.lat() + gps.location.lng() + gps.altitude.meters() + gps.speed.kmph();
  }

  Serial.println("Reading sensors...");
  temp.read();
  lum.read();
  uvMl.read();

  observation.temperature = temp.t;
  observation.luminosity = lum.lux;
  observation.uvlight = uvMl.uvindex;
  observation.checksum += temp.t + lum.lux + uvMl.uvindex;

  Serial.println("Checksum: ");
  Serial.println(observation.checksum);

  infrared.read();
  accelerometer.read();
  gyroscope.read();
  magnetometer.read();

  // Send data to the SD Card logger
  Serial.println("Sending logger...");
  
  logger.print(millis());
  logger.print(",");
  if (gps.time.isValid())
    {
      if (gps.time.hour() < 10) logger.print(F("0"));
      logger.print(gps.time.hour());
      logger.print(F(":"));
      if (gps.time.minute() < 10) logger.print(F("0"));
      logger.print(gps.time.minute());
      logger.print(F(":"));
      if (gps.time.second() < 10) logger.print(F("0"));
      logger.print(gps.time.second());
    }
    else
    {
      Serial.print(F("INVALID"));
    }
  logger.print(",");
  logger.print(gps.location.lat());
  logger.print(",");
  logger.print(gps.location.lng());
  logger.print(",");
  logger.print(gps.altitude.meters());
  logger.print(",");
  logger.print(gps.speed.kmph());
  logger.print(",");
  logger.print(temp.t);
  logger.print(",");
  logger.print(lum.lux);
  logger.print(",");
  logger.print(uvMl.uvindex);
  logger.print(",");
  logger.print(infrared.t);
  logger.print(",");
  logger.print(accelerometer.x);
  logger.print(",");
  logger.print(accelerometer.y);
  logger.print(",");
  logger.print(accelerometer.z);
  logger.print(",");
  logger.print(gyroscope.x);
  logger.print(",");
  logger.print(gyroscope.y);
  logger.print(",");
  logger.print(gyroscope.z);
  logger.print(",");
  logger.print(magnetometer.x);
  logger.print(",");
  logger.print(magnetometer.y);
  logger.print(",");
  logger.println(magnetometer.z);

  Serial.print("Sending ");
  Serial.print(sizeof(Observation));
  Serial.println(" byte packet...");

  // Send data on the LoRa radio
  Serial.println("Sending radio...");

  rf95.send((uint8_t *)&observation, sizeof(Observation));
  rf95.waitPacketSent();
}*/
