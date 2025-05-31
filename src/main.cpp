#include <Arduino.h>
#include <Wire.h>

#define RDAW 0x11  // 0b0010000
#define RDAR 0x10  //Not used -> As it increment by one every time we accesse the register.
#define SUP 25    // Channel Up


void writeRegister(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(RDAW);
  Wire.write(reg);
  Wire.write(highByte(value));
  Wire.write(lowByte(value));
  Wire.endTransmission();
}

void setup() {
  Wire.begin(21, 22);  // SDA, SCL (for ESP32)
  pinMode(SUP,INPUT_PULLDOWN);
  Serial.begin(115200);
  writeRegister(0x02,0b1111000000001001); //  1111000000001001 0xF009
  delay(100);
  writeRegister(0x05,0b1000100010001111); //  1000100010001111 0x888F
  delay(100);  // Wait for tuning 
  writeRegister(0x03,0b0000100010010000);  // 000000100010010000 = 0x890 // 90.4Mhz
  delay(100);
  writeRegister(0x04,0b0000001001000000); //0000001001000000 = 0x240
  delay(100);
  writeRegister(0x06,0b0000100001100000); //0000100001100000 = 0x860
  delay(100);

    Serial.println("RDA5807FP initialized.");
}

/**
 * 0x890 = 90.4Mhz
 * 0x490 = 88.8Mhz
 * 0x1290 = 94.4Mhz
 * 0x2810 = 103.0 Mhz
 */

uint16_t ch[4] = {0x890,0x490,0x1290,0x2810};
int chS = 0;    // Channel Select
void loop() {
  // Main loop
    if(digitalRead(SUP) == HIGH){
        delay(50);
        if(digitalRead(SUP) == HIGH){
            chS += 1;
            if(chS > 3){chS = 0;}
            Serial.println(chS);
            writeRegister(0x03,ch[chS]);  // 0001000100010000 = 0x1110 // 90.4Mhz
            delay(100);
        }
         
    }
}



 