//
// buzzer.h
//
#define BUZZER_PIN 3
#define BUZZER_ENABLED true

void initBuzzer() {
  Serial.println("Initializing piezo buzzer...");
  pinMode(BUZZER_PIN, BUZZER_ENABLED ? OUTPUT : INPUT);
}

void beep() {
  tone(BUZZER_PIN, 512, 50);
}

void testBuzzer() {
  beep();
}
