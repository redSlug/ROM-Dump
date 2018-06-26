#include<Arduino.h>

int output_enabled = 13;
int delay_time = 100; // 100 * 2 *  (2**19) microseconds to seconds = 104 seconds

// serial to parallel
int stp_clock = 0;
int stp_data = 1;

void setup() {
  for (int i = 0; i < 11; i++) {
    pinMode(i + 2, OUTPUT);
  }
  pinMode(output_enabled, OUTPUT);
  
  digitalWrite(output_enabled, HIGH);
  
  pinMode(stp_clock, OUTPUT);
  pinMode(stp_data, OUTPUT);
}

void write_8_bit_num(int n) {
  for (int i = 0; i < 8; i++) {
      delayMicroseconds(delay_time);
      digitalWrite(stp_clock, LOW);
      if (n % 2 == 1) {
        digitalWrite(stp_data, HIGH);
      } else {
        digitalWrite(stp_data, LOW);
      }
      delayMicroseconds(delay_time);
      digitalWrite(stp_clock, HIGH);
      n = n >> 1;  
  }
}

void loop() {    

  write_8_bit_num(0);
  delay(500);
  write_8_bit_num(255);
  delay(500);
  write_8_bit_num(0);
  delay(500);
  write_8_bit_num(255);
  delay(500);

  unsigned long highest_address_we_can_afford = 1L << (11 + 8);

  for (unsigned long i = 0; i<highest_address_we_can_afford; i++) {
    
    // A0 to A10 are coming from the arduino directly
    for (unsigned long j = 0; j < 11; j++) {
      if (i & (1 << j)) {
        digitalWrite(j + 2, HIGH);
      } else {
        digitalWrite(j + 2, LOW);
      }   
    }  

     // writing hightest bits, A11 to A18 to MC140158
     // A19 goes to ground
     unsigned long low_bits_overflow_threshold = 1L << 11;
     if (i % low_bits_overflow_threshold == 0) {
      // only update higher bits when the value gets updated
      write_8_bit_num(i>>11); 
     }
     

    delayMicroseconds(delay_time);
    digitalWrite(output_enabled, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(output_enabled, HIGH);
  }
}


  // SNES  32pin ROM mapping https://www.caitsith2.com/snes/flashcart/cart-chip-pinouts.html#rom

  // SIDE 1
  // pin1   MC140158 4          A17
  // pin2   MC140158 5          A18 (highest bit) 
  // pin3   MC140158 10         A15  
  // pin4   MC140158 11         A12
  // pin5   Arduino 9           A7
  // pin6   Arduino 8           A6
  
  // pin7   Arduino 7           A5
  // pin8   Arduino 6           A4
  // pin9   Arduino 5           A3
  // pin10  Arduino 4           A2
  // pin11  Arduino 3           A1
  // pin12  Arduino 2           A0

  // pin13  Saleae              D0  (data - lowest output pin)
  // pin14  Saleae                    D1
  // pin15  Saleae                    D2
  // pin16      ground          Vss


  // SIDE 2
  // pin32                      Vcc (5V_ energy)
  // pin31    Arduino 13        /OE (Output Enabled)
  // pin30    Ground            A19 (not used, not enough output pins) <<<<-->>>> now mapping to +
  // pin29    MC140158 13       A14
  // pin28    MC140158 12       A13
  // pin27    Arduino 10        A8
  // pin26    Arduino 11        A9
  // pin25    MC140158 2        A11 
  // pin24    MC140158 3        A16
  // pin23    Arduino 12        A10
  // pin22    Ground            /CS (Chip select - if you have multiple chips on same wire, can share bus lines, feature of SNES ROM chip we have to set to 0 because we're not using it)

  // pin21  Saleae                    D7
  // pin20                      D6
  // pin19                      D5
  // pin18                      D4
  // pin17                      D3


  // Arduino            MC140158        SNES           
  // 0 -> STP Clock
  // 1 -> STP Data        ->            A11 -> A18 
  // 0 -> STP Clock      MC140158 pin 9              
  // 1 -> STP Data              MC140158 pin 7     (controls pins A11 through A18)

  // MC140158  SNES
  // 2          A11        
  // 11         A12
  // 12         A13
  // 13         A14  

  // 10         A15        
  // 3          A16
  // 4          A17
  // 5          A18  

  // Ard  SNES
  // 2 -> A0
  // 3 -> A1
  // 4 -> A2
  // 5 -> A3
  // 6 -> A4
  // 7 -> A5
  // 8 -> A6
  // 9 -> A7
  // 10 -> A8
  // 11 -> A9
  // 12 -> A10
  // 13 -> OE / Output Enabled
