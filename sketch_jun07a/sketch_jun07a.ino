  #include<Arduino.h>

int output_enabled = 13;
int a_19_snes = 14;
int delay_time = 10; 
// total runtime 100 * 2 (delays) *  (2**20) microseconds to seconds = 208 seconds
// note this estimate becomes inaccurate for shorter delays, as we approach CPU clock period

// serial to parallel
int stp_clock = 0;
int stp_data = 1;

void setup() {
  for (int i = 0; i < 11; i++) {
    pinMode(i + 2, OUTPUT);
  }
  pinMode(output_enabled, OUTPUT);
  
  digitalWrite(output_enabled, HIGH);

  pinMode(a_19_snes, OUTPUT);
  
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
  delay(1000);
  write_8_bit_num(255);
  delay(1000);
  write_8_bit_num(0);
  delay(1000);
  write_8_bit_num(255);
  delay(1000);

  unsigned long highest_address_we_can_afford = 1L << (11 + 8 + 1);

  for (unsigned long i = 0; i<highest_address_we_can_afford; i++) {
    
    // A0 to A10 are coming from the Arduino directly
    for (unsigned long j = 0; j < 11; j++) {
        digitalWrite(j + 2, (i & (1L << j)) == 0 ? LOW : HIGH);
    }

     // writing A11 to A18 via MC140158
     unsigned long low_bits_overflow_threshold = 1L << 11;
     if (i % low_bits_overflow_threshold == 0) {
      // only update higher bits when the value gets updated
      write_8_bit_num(i>>11); 
     }

    // writing A19 highest bit
    digitalWrite(a_19_snes, (i & (1L << 19)) == 0 ? LOW : HIGH);
     
    delayMicroseconds(delay_time);
    digitalWrite(output_enabled, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(output_enabled, HIGH);
  }
}

  // SNES  32pin ROM mapping https://www.caitsith2.com/snes/flashcart/cart-chip-pinouts.html#rom
  // CS Chip select - if you have multiple chips on same wire,
  // can share bus lines, feature of SNES ROM chip set to 0 because not using

  // SIDE 1
  // pin1   MC140158 4          A17
  // pin2   MC140158 5          A18 
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
  // pin14  Saleae              D1
  // pin15  Saleae              D2
  // pin16  ground              Vss

  // SIDE 2
  // pin32  5V_ energy          Vcc
  // pin31  Arduino 13          /OE (Output Enabled)
  // pin30  Arduino A0        A19 (highest bit)
  // pin29  MC140158 13         A14
  // pin28  MC140158 12         A13
  // pin27  Arduino 10          A8
  // pin26  Arduino 11          A9
  // pin25  MC140158 2          A11
  // pin24  MC140158 3          A16
  // pin23  Arduino 12          A10
  // pin22  Ground              /CS
  // pin21  Saleae              D7
  // pin20  Saleae              D6
  // pin19  Saleae              D5
  // pin18  Saleae              D4
  // pin17  Saleae              D3

  // Arduino     MC140158           SNES
  // 0           STP Clock pin 9
  // 1           STP Data  pin 7    (controls pins A11 through A18)

  // MC140158  SNES
  // 2          A11        
  // 11         A12
  // 12         A13
  // 13         A14  

  // 10         A15        
  // 3          A16
  // 4          A17
  // 5          A18  

  // Arduino    SNES
  // 2          A0
  // 3          A1
  // 4          A2
  // 5          A3
  // 6          A4
  // 7          A5
  // 8          A6
  // 9          A7
  // 10         A8
  // 11         A9
  // 12         A10
  // 13         OE / Output Enabled
