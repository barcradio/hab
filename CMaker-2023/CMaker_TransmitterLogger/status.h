//
// status.h - use serial and single WS2812B to indicate system status.
// Q. Should we combine the buzzer, LED, and camera light in this class?
//

//#include <FastLED.h>

#define LED_PIN     1
#define NUM_LEDS    1

enum PayloadStatus {
  ERROR_CODE,  // failed - red blink error code and display info on serial port 
  SETUP,    // initializing systems - blink white
  GPS_SEEK,  // booting and acquiring GPS lock - blink yellow
  READY,    // everything GO - blink green
  INFLIGHT, // LED off
  LANDED    // off or blink green
};

enum PayloadError {
  // Add failure conditions and an associated integer for how many
  // times to blink the red light to display the error.
  NO_ERROR = 0,
  RANDOM_FAILURE = 1,
  LOW_BATTERY = 2
};

PayloadStatus payloadStatus = SETUP;
PayloadError payloadErrorCode = NO_ERROR;

setPayloadStatus(PayloadStatus status) {
  payloadStatus = status;
}

setPayloadError(PayloadError errorCode) {
  payloadStatus = ERROR_CODE;
  payloadErrorCode = errorCode;
}

/*
CRGB statusLED;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void setStatus(payloadStatus status, string message) {
  
}

void showStatus() {
  // in progress ... 
  //    for error status, go into infinite loop blinking error code.
  //    
  switch (status) {
  
  }
  leds[0] = CRGB(255, 0, 0); // red
  FastLED.show();
  delay(500);
  leds[0] = CRGB(0, 0, 0); // off
  FastLED.show();
}


*/
