// CMaker_Receiver.ino
//
// High Altitude Balloon class
// Cache Makers and Bridgerland Amateur Radio Club

// Update values here to be displayed during startup
#define showOwner true
#define myName "Cache Makers"
#define myPhone "(435) 915-6253"


// To compile this code for the ground stations ...
//
// 1. Make sure required libraries are in ~/arduino/libraries
//        libraries/LoRa
//        libraries/U8g2_Arduino
// 2. in Arduino IDE, select Tools->Board: "ESP32 Arduino" -> "TTGO LoRa32-OLED"
// 3. in Arduino IDE, select Tools->Board Revision: "TTGO LoRa32 V2.1 (1.6.1)"
// 4. in Arduino IDE, select Tools->Port: <COM port for receiver>
// 5. compile and upload
//
// note: Serial baud rate is set in initBoard() to 115200 baud

#include <LoRa.h>
#include "board.h"
#include "telemetry.h"
#include "webServer.h"

#define MAX_PKT_LENGTH           255
uint8_t recvBuf[MAX_PKT_LENGTH];
uint8_t len = sizeof(recvBuf);
int recvLen = 0;

char dbuf[256]; // display buffer
float m2ft = 3.28084; // convert meters to feet
float kph2mph = 0.621371; // convert KPH to MPH
float C2F(float C) { return C * 1.8 + 32; }

void getAltString() {
  int alt = telemetry.gpsalt * m2ft;
  if (alt >= 1000)
    snprintf(dbuf, sizeof(dbuf), "ALT: %d,%03d ft", alt/1000, alt%1000);
  else
    snprintf(dbuf, sizeof(dbuf), "ALT: %d ft", alt);
}

int lineHt = 16; // Line height for OLED display with large font
int L1 = 12;
int L2 = L1 + lineHt;
int L3 = L2 + lineHt;
int L4 = L3 + lineHt;

void updateDisplay(int page) {
  if (page == 0) {
    u8g2->clearBuffer();
    snprintf(dbuf, sizeof(dbuf), "Received: %d", ++packetCount);
    u8g2->drawStr(0, L1, dbuf);
    snprintf(dbuf, sizeof(dbuf), "SIZE: %d", recvLen);
    u8g2->drawStr(0, L2, dbuf);
    snprintf(dbuf, sizeof(dbuf), "RSSI: %i", LoRa.packetRssi());
    u8g2->drawStr(0, L3, dbuf);
    snprintf(dbuf, sizeof(dbuf), "SNR: %.1f dB", LoRa.packetSnr());
    u8g2->drawStr(0, L4, dbuf);
    u8g2->sendBuffer();
  } else if (page == 1) {
    u8g2->clearBuffer();
    snprintf(dbuf, sizeof(dbuf), "Received: %d", ++packetCount);
    u8g2->drawStr(0, L1, dbuf);
    getAltString();
    u8g2->drawStr(0, L2, dbuf);
    snprintf(dbuf, sizeof(dbuf), "SPEED: %.0f MPH", telemetry.gpsspeed * kph2mph);
    u8g2->drawStr(0, L3, dbuf);
    snprintf(dbuf, sizeof(dbuf), "TEMP: %.1f F", C2F(telemetry.temperature));
    u8g2->drawStr(0, L4, dbuf);
    u8g2->sendBuffer();
  } else if (page == 2) {
    u8g2->clearBuffer();
    snprintf(dbuf, sizeof(dbuf), "LAT: %0.6f", telemetry.gpslat);
    u8g2->drawStr(0, L1, dbuf);
    snprintf(dbuf, sizeof(dbuf), "LON: %0.6f", telemetry.gpslon);
    u8g2->drawStr(0, L2, dbuf);
    snprintf(dbuf, sizeof(dbuf), "BATT: %.2f Volts", telemetry.vBatt);
    u8g2->drawStr(0, L3, dbuf);
    snprintf(dbuf, sizeof(dbuf), "B_TEMP: %.1f F", C2F(telemetry.tempBatt));
    u8g2->drawStr(0, L4, dbuf);
    u8g2->sendBuffer();
  }
}

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);
    u8g2->setFont(u8g2_font_fur11_tf);

    Serial.println("LoRa Receiver");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    // initialize web server for control configuration and data views
    if (!initWebServer()) {
        Serial.println("Starting Web Server failed!");
        while (1);
    }
}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        recvLen = 0;
        while (LoRa.available()) {
            recvBuf[recvLen++] = (uint8_t)LoRa.read();
        }

        Serial.print("Received packet with ");
        Serial.print(recvLen);
        Serial.println(" bytes");
        
        // print RSSI (Received Signal Strength Indicator) of packet
        Serial.print("\tRSSI = ");
        Serial.println(LoRa.packetRssi());
        // print SNR (signal to noise ratio) of packet
        Serial.print("\tSNR = ");
        Serial.print(LoRa.packetSnr());
        Serial.println(" db");

        Serial.print("Size of expected telemetry is ");
        Serial.print(sizeof(Telemetry));
        Serial.println(" bytes");

        if (recvLen == sizeof(Telemetry)) {
          // Copy the received buffer values directly into the Telemetry data structure
          memcpy(&telemetry, recvBuf, sizeof(Telemetry));

          checksum = calculateTelemetryChecksum(&telemetry);

          Serial.print("Calculated Checksum: ");
          Serial.println(checksum);
          
          Serial.print("  Received Checksum: ");
          Serial.println(telemetry.checksum);
          
          Serial.println("-----DATA--------------");
          Serial.print("  ID: ");
          Serial.println(telemetry.id, HEX);
          Serial.print("  type: ");
          Serial.println(telemetry.packetType);
          Serial.print("  status: ");
          Serial.println(telemetry.payloadStatus);
          Serial.print("  Latitude: ");
          Serial.println(telemetry.gpslat);
          Serial.print("  Longitude: ");
          Serial.println(telemetry.gpslon);
          Serial.print("  Altitude: ");
          Serial.println(telemetry.gpsalt);
          Serial.print("  Speed: ");
          Serial.println(telemetry.gpsspeed);
          Serial.print("  Temp C: ");
          Serial.println(telemetry.temperature);
          Serial.print("  Temp F: ");
          Serial.println(telemetry.temperature*9/5+32.0);
          Serial.print("  Luminosity: ");
          Serial.println(telemetry.luminosity);
          Serial.print("  UV Light: ");
          Serial.println(telemetry.uvlight);

          Serial.print("  battery voltage: ");
          Serial.println(telemetry.vBatt);
          Serial.print("  payload internal temperature: ");
          Serial.println(telemetry.tempInt*9/5+32);
          Serial.print("  battery temperature: ");
          Serial.println(telemetry.tempBatt*9/5+32);
          Serial.print("  atmospheric temperature: ");
          Serial.println(telemetry.tempExt*9/5+32);

          Serial.println("-----------------------\n");
          
          if (checksum == telemetry.checksum) {
            updateWebserverData();
          }

          updateDisplay(1);
          delay(5000);
          updateDisplay(2);
        }
    }
}
