#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

// High-Altitude Balloon project by Cache Makers and Bridgerland Amateur Radio Club.
// These definitions are for the T3 V1.6 LoRa32-OLED used in the ground station receivers.
//
// Note: the arduinoLoRa Library only supports SX1276/Sx1278, and does Not support SX1262
// RadioLib Library supports also SX1262/SX1268 see https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/tree/master/examples/RadioLibExamples
//


// The LoRa frequency band for use in the United States is 915 MHz
// Frequency options are: 433E6,470E6,868E6,915E6
#define LoRa_frequency      915E6

#define UNUSE_PIN                   (0)

#define I2C_SDA                     21
#define I2C_SCL                     22
#define OLED_RST                    UNUSE_PIN

#define RADIO_SCLK_PIN              5
#define RADIO_MISO_PIN              19
#define RADIO_MOSI_PIN              27
#define RADIO_CS_PIN                18
#define RADIO_DIO0_PIN               26
#define RADIO_RST_PIN               23
#define RADIO_DIO1_PIN              33
#define RADIO_BUSY_PIN              32

#define SDCARD_MOSI                 15
#define SDCARD_MISO                 2
#define SDCARD_SCLK                 14
#define SDCARD_CS                   13

#define BOARD_LED                   25
#define LED_ON                      HIGH
#define LED_OFF                      LOW

#define ADC_PIN                     35

#define HAS_SDCARD
#define HAS_DISPLAY

// includes for SD Card reader/writer
#include <SD.h>
#include <FS.h>

// NOTE: Warning: Multiple libraries were found for "SD.h"
//    Used: C:\Users\Carl\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6\libraries\SD
//    Not used: C:\Program Files (x86)\Arduino\libraries\SD


// includes for onboard OLED screen
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;

#define initPMU()
#define disablePeripherals()

SPIClass SDSPI(HSPI);


void initBoard()
{
    Serial.begin(115200);
    Serial.println("initBoard");
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
    Wire.begin(I2C_SDA, I2C_SCL);

#ifdef HAS_GPS
    Serial1.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
#endif

#if OLED_RST
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, HIGH); delay(20);
    digitalWrite(OLED_RST, LOW);  delay(20);
    digitalWrite(OLED_RST, HIGH); delay(20);
#endif

    initPMU();


#ifdef BOARD_LED
    /*
    * T-BeamV1.0, V1.1 LED defaults to low level as trun on,
    * so it needs to be forced to pull up
    * * * * */
#if LED_ON == LOW
    gpio_hold_dis(GPIO_NUM_4);
#endif
    pinMode(BOARD_LED, OUTPUT);
    digitalWrite(BOARD_LED, LED_OFF);
#endif


#ifdef HAS_DISPLAY
    Wire.beginTransmission(0x3C);
    if (Wire.endTransmission() == 0) {
        Serial.println("Started OLED");
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFlipMode(0);
        u8g2->setFontMode(1); // Transparent
        u8g2->setDrawColor(1);
        u8g2->setFontDirection(0);
        u8g2->firstPage();
        do {
            u8g2->setFont(u8g2_font_ncenB12_tr);
            u8g2->drawStr(0, 30, "Cache Makers");
            u8g2->drawHLine(2, 35, 47);
            u8g2->drawHLine(3, 36, 47);
            u8g2->drawVLine(45, 32, 12);
            u8g2->drawVLine(46, 33, 12);
            u8g2->setFont(u8g2_font_inb19_mf);
            u8g2->drawStr(58, 60, "BARC");
        } while ( u8g2->nextPage() );
        u8g2->sendBuffer();
        u8g2->setFont(u8g2_font_fur11_tf);
        delay(3000);
        
        if (showOwner) {
          u8g2->clearBuffer();
          u8g2->firstPage();
          do {
              u8g2->drawStr(0, 30, myName);
              u8g2->drawHLine(0, 40, 124);
              u8g2->drawStr(0, 60, myPhone);
          } while ( u8g2->nextPage() );
          u8g2->sendBuffer();
          u8g2->setFont(u8g2_font_fur11_tf);
          delay(3000);
        }
    }
#endif


#ifdef HAS_SDCARD
    pinMode(SDCARD_MISO, INPUT_PULLUP);
    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
    if (u8g2) {
        u8g2->clearBuffer();
    }

    if (!SD.begin(SDCARD_CS, SDSPI)) {

        Serial.println("setupSDCard FAIL");
        if (u8g2) {
            do {
                u8g2->setCursor(0, 16);
                u8g2->println( "SDCard FAILED");;
            } while ( u8g2->nextPage() );
        }

    } else {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        if (u8g2) {
            do {
                u8g2->setCursor(0, 16);
                u8g2->print( "SDCard:");;
                u8g2->print(cardSize / 1024.0);;
                u8g2->println(" GB");;
            } while ( u8g2->nextPage() );
        }

        Serial.print("setupSDCard PASS . SIZE = ");
        Serial.print(cardSize / 1024.0);
        Serial.println(" GB");
    }
    if (u8g2) {
        u8g2->sendBuffer();
    }
    delay(3000);
#endif

    if (u8g2) {
        u8g2->clearBuffer();
        do {
            u8g2->setCursor(0, 16);
            u8g2->println( "Waiting to receive data");;
        } while ( u8g2->nextPage() );
    }


}
