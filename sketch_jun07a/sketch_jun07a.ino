#include<Arduino.h>

//int output_enabled = 10;

// serial to parallel
int stp_clock = 2;
int stp_data = 3;
int tmp_signal = 4;

void setup() {
//  for (int i = 0; i < 8; i++) {
//    pinMode(i + 2, OUTPUT);
//  }
//  pinMode(output_enabled, OUTPUT);
//  digitalWrite(output_enabled, HIGH);
  pinMode(stp_clock, OUTPUT);
  pinMode(stp_data, OUTPUT);
}

void write_8_bit_num(int n) {
  for (int i = 0; i < 8; i++) {
      delay(10);
      digitalWrite(stp_clock, LOW);
      if (n % 2 == 1) {
        digitalWrite(stp_data, HIGH);
      } else {
        digitalWrite(stp_data, LOW);
      }
      delay(10);
      digitalWrite(stp_clock, HIGH);
      n = n >> 1;  
  }
}

void loop() {
  for (int i = 0; i<255; i++) {
      write_8_bit_num(i);
      digitalWrite(tmp_signal, HIGH);
      delay(100);

      digitalWrite(tmp_signal, LOW);

      delay(100);
  }
  
//  for (int i = 0; i<255; i++) {
//    for (int j = 0; j < 8; j++) {
//      if (i & (1 << j)) {
//        digitalWrite(j + 2, HIGH);
//      } else {
//        digitalWrite(j + 2, LOW);
//      }   
//    }    
//
//    delay(1);
//    digitalWrite(output_enabled, LOW);
//    delay(5);
//    digitalWrite(output_enabled, HIGH);
//  }
}
