//
// lora.h
//
#include <RH_RF95.h>

// Radio parameters
#define RFM95_CS 4
#define RFM95_RST 3
#define RFM95_INT 2
#define RF95_FREQ 915.0  // Transmitter 2 - Red

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void initLoRa()
{
  Serial.println("Initializing LoRa radio...");
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

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
}

void sendLoRa(uint8_t *data, size_t dataSize) {
  rf95.send(data, dataSize);
  rf95.waitPacketSent();
}
