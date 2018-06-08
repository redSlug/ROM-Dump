#include<Arduino.h>

int output_enabled = 10;


void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(i + 2, OUTPUT);
  }
  pinMode(output_enabled, OUTPUT);
  digitalWrite(output_enabled, HIGH);
}

void loop() {
  for (int i = 0; i<255; i++) {
    for (int j = 0; j < 8; j++) {
      if (i & (1 << j)) {
        digitalWrite(j + 2, HIGH);
      } else {
        digitalWrite(j + 2, LOW);
      }   
    }    

    delay(1);
    digitalWrite(output_enabled, LOW);
    delay(5);
    digitalWrite(output_enabled, HIGH);
  }
}
