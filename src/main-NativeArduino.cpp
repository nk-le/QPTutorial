#include <Arduino.h>
#include <Defines.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(0);

  for (int i = 0; i < 20; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    Serial.println("Tick...");
  }

  CPU_RESTART
  //_reboot_Teensyduino_();  
}

void loop() {
  // put your main code here, to run repeatedly:
}