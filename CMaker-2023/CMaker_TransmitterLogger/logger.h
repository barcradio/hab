//
// logger.h
//
#include <ArdusatSDK.h>
#include <Wire.h>

#define LOG_RX        13 // SoftwareSerial receive (NOT USED - DON'T ASSIGN ELSEWHERE)
#define LOG_TX        18 // SoftwareSerial transmit to SD Card

ArdusatSerial logger(SERIAL_MODE_SOFTWARE, LOG_RX, LOG_TX);

void initLogger() {
  Serial.println("Initializing data logger...");
  logger.begin(9600);
  delay(100);
}

void writeLogHeader() {
  logger.println("BARC/Cache Makers 2023 High Altitude Balloon Launch Telemetry");
  logger.println("millis,GPS time,latitude,longitude,altitude,speed,temp,luminosity,UV,infrared,accel.x,accel.y,accel.z,gyro.x,gyro.y,gyro.z,mag.x,mag.y,mag.z,status,vBatt,tBatt,tInt,tExt");
}

void writeLogEntry() {
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
  logger.print(",");
  logger.print(gps.location.lat(), 6);
  logger.print(",");
  logger.print(gps.location.lng(), 6);
  logger.print(",");
  logger.print(gps.altitude.meters());
  logger.print(",");
  logger.print(gps.speed.kmph());
  logger.print(",");
  logger.print(temp.t);
  logger.print(",");
  logger.print(lum.lux);
  logger.print(",");
  logger.print(uv.uvindex);
  logger.print(",");
  logger.print(ir.t);
  logger.print(",");
  logger.print(acc.x);
  logger.print(",");
  logger.print(acc.y);
  logger.print(",");
  logger.print(acc.z);
  logger.print(",");
  logger.print(gyro.x);
  logger.print(",");
  logger.print(gyro.y);
  logger.print(",");
  logger.print(gyro.z);
  logger.print(",");
  logger.print(mag.x);
  logger.print(",");
  logger.print(mag.y);
  logger.print(",");
  logger.print(mag.z);
  logger.print(",");
  if (payloadStatus == ERROR_CODE) {
    logger.print("ERR_");
    logger.print(payloadErrorCode);
  } else {
    logger.print(payloadStatus);
  }
  logger.print(",");
  logger.print(batteryVoltage);
  logger.print(",");
  if (tempBattery != -127)
    logger.print(tempBattery);
  logger.print(",");
  if (tempInterior != -127)
    logger.print(tempInterior);
  logger.print(",");
  if (tempExterior != -127)
    logger.print(tempExterior);
  logger.println(); // end of entry
}
