// ardusat_10dof_receiver.ino
//
// Program to receive "10 degrees of freedom" data from ardusat sensors and transmitter.
// This reciever recieves the transmitted accelerometer, gyro, magnetometer, and altitude
// values from the transmitter and then formats for serial output.
//
// The serial output is read by Ardusat's Experiment Hub pages.
//
// This sketch is a modified version of the sketch: Arduino9x_RX from the excellent
// library: http://www.airspayce.com/mikem/arduino/RadioHead/

#include <SPI.h>
#include <Wire.h>
#include <RH_RF95.h>
#include <LiquidCrystal_I2C.h>

// Set up LCD
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // Interrupt may be 0x38

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Items to be received...
// 4 bytes per float * 7 values + checksum value = 32 bytes
struct Observation {
  float gpslat, gpslon, gpsalt, gpsspeed;
  float temperature;
  float luminosity;
  float uvlight;
  float checksum;
};

#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 9

long timestamp;

int i;

unsigned long start, finished, elapsed;

void initPins()
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
}

void initSerial()
{
  Serial.begin(115200);
  delay(100);
}

void blinkHello()
{
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
}

void initRadio()
{
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  
  rf95.init();
  rf95.setFrequency(RF95_FREQ);

  blinkHello();
}

/* Add up all the values in the Observation data structure to get a "checksum" value
   that can later be compared with the checksum value inside the observation. If the
   checksum values are equal, then we have a high level of confidence that the data
   we received over the radio reflects accurate sensor readings.
*/
float calculateChecksumForObservation(Observation* observation) {
  return
    observation->gpslat + observation->gpslon + observation->gpsalt + observation->gpsspeed +
    observation->temperature +
    observation->luminosity +
    observation->uvlight;
}

void setup() 
{
  start=millis();
  
  Serial.println(F("Initializing."));
  initPins();
  initSerial();
  initRadio();
  Serial.println(F("Initialize complete."));

  // Start LCD
  lcd.begin (20, 4);
  lcd.backlight();
  // Switch on the backlight
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();
  lcd.print("    Cache Makers");
  lcd.setCursor(0, 1);
  lcd.print("   Balloon Tracker");
}

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

Observation observation;

float checksum;

void loop()
{
  start = micros();
  if ((start - finished)>=1000000){ // check for 1 second passed
    finished = finished + 1000000; //
    elapsed = elapsed +1;
     Serial.println(elapsed);
  }
  
  lcd.setCursor(15, 3);
  lcd.print(elapsed);
  
  if (rf95.available())
  {
    // Should be a message waiting for us now

    if (rf95.recv(buf, &len))
    {
      Serial.println(F("----------------------"));
      Serial.print("#");
      Serial.print(i);
      i++;
      Serial.print(F(" Received "));
      Serial.print(len);
      Serial.println(F(" bytes"));

      digitalWrite(LED, HIGH);

      if (len >= sizeof(Observation)) {
        // Copy the received buffer values directly into our Observation data structure
        memcpy(&observation, buf, sizeof(Observation));

        checksum = calculateChecksumForObservation(&observation);
/*
        Serial.println("Checksum: ");
        Serial.println(checksum);

        Serial.println("Observation checksum: ");
        Serial.println(observation.temperature);
*/
        if (checksum == observation.checksum) {
          // Direct Measurements

          Serial.print(F("Lat: "));
          Serial.println(observation.gpslat);

          Serial.print(F("Lon: "));
          Serial.println(observation.gpslon);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Loc: ");
          lcd.print(observation.gpslat);
          lcd.print(" ");
          lcd.print(observation.gpslon);
          

          Serial.print(F("Alt: "));
          Serial.println(observation.gpsalt);

          lcd.setCursor(0, 1);
          lcd.print("Alt:");
          lcd.print(observation.gpsalt*3.28,0);

          Serial.print(F("Speed: "));
          Serial.println(observation.gpsspeed);

          lcd.print(F(" Speed:"));
          lcd.print(observation.gpsspeed*.62137,0);

          Serial.print(F("Temp: "));
          Serial.println(observation.temperature);

          lcd.setCursor(0, 2);
          lcd.print("Temp:");
          lcd.print(observation.temperature*9/5 +32,0);

          Serial.print(F("Lum: "));
          Serial.println(observation.luminosity);

          lcd.print(" Lum:");
          lcd.print(observation.luminosity,0);
          
          Serial.print(F("UV: "));
          Serial.println(observation.uvlight);

          lcd.setCursor(0, 3);
          lcd.print("UV:");
          lcd.print(observation.uvlight);

          // Reset timer
          elapsed = 0;

        } else {
          // uh oh! skip a bad value
          Serial.println(F("Bad checksum value"));
        }
      } else {
        // strange, the packet wasn't as large as we expect it to be
          Serial.println(F("Bad packet size"));
      }
    } else {
      Serial.println(F("Problem with recv"));
    }
}

  digitalWrite(LED, LOW);
}
