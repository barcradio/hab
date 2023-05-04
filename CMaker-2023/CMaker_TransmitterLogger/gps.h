//
// gps.h
//
#include <ArdusatSDK.h>
#include <TinyGPS++.h>

// GPS Pins
static const int RXPin = 0;
static const int TXPin = 12; // SoftwareSerial tx (NOT USED - DON'T ASSIGN ELSEWHERE)
static const uint32_t GPSBaud = 4800;

// Logging interval in ms. Due to sensor delays, 7300ms = ~10 seconds
long interval = 7300;
long previousMillis = 0;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS

float gpslat, gpslon, gpsalt, gpsspeed;


void initGPS() {
    Serial.println("Initializing GPS...");
    ss.begin(GPSBaud);
}

char gpsBuffer[1024];

void readGPS() {
  unsigned long currentMillis = millis();
  
  // If GPS is available, then every interval collect sensor data and transmit 
  int i=0;
  while (ss.available() > 0) {
    char c = ss.read();
    gpsBuffer[i++] = c; // save for display
    if (gps.encode(c)) {
      gpsBuffer[i] = '\0';
      Serial.println(gpsBuffer); // display NMEA sentenses from GPS
      if (gps.location.isValid()) {
        gpslat = gps.location.lat();
        gpslon = gps.location.lng();
        gpsalt = gps.altitude.meters();
        gpsspeed = gps.speed.kmph();
      }
    }
  }
/*  
  while (gps.encode(ss.read()) && currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;  
    // logInfo();
    if (gps.location.isValid()) {
      gpslat = gps.location.lat();
      gpslon = gps.location.lng();
      gpsalt = gps.altitude.meters();
      gpsspeed = gps.speed.kmph();
    }
  }
*/
}
void testGPS() {
  unsigned long currentMillis = millis();
  
  // If GPS is available, then every interval collect sensor data and transmit
  // read all available bytes and push to gps.
  int i=0;
  Serial.print(ss.available());
  Serial.println(" bytes available at GPS");
  while (ss.available() > 0) {
    char c = ss.read();
    gpsBuffer[i++] = c; // save for display
    if (gps.encode(c)) {
      gpsBuffer[i] = '\0';
      Serial.println(gpsBuffer); // display NMEA sentenses from GPS
      if (currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;
        // logInfo(); // log data to onboard sdcard.
      }
    }
  }

  if (gps.location.isValid()) {
    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Lon: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print(", Alt: ");
//    Serial.print(gps.altitude.meters());
//    Serial.print(" m = ");
    Serial.print(gps.altitude.meters()*3.281);
    Serial.println(" ft");
    /*
    observation.gpslat = gps.location.lat();
    observation.gpslon = gps.location.lng();
    observation.gpsalt = gps.altitude.meters();
    observation.gpsspeed = gps.speed.kmph();
    observation.checksum += gps.location.lat() + gps.location.lng() + gps.altitude.meters() + gps.speed.kmph();
    */
  } else {
    Serial.println("Acquiring GPS lock... no lock");
  }


}
