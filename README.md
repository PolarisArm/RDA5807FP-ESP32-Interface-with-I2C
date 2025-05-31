# RDA5807FP-ESP32-Interface-with-I2C
Interfacing RDA5807FP with ESP32 via I2C

This project demonstrates how to interface the **RDA5807FP** FM radio receiver chip with an **ESP32** using I2C communication. It tunes to predefined FM channels and switches between them using a button.

## 📡 Features

- Initializes RDA5807FP via I2C
- Tunes to preset FM frequencies
- Supports 4 hardcoded channels
- Uses a GPIO button to cycle through channels
- Serial output for debugging

## 🛠️ Hardware Requirements

| Component     | Description                  |
|---------------|------------------------------|
| ESP32         | Any ESP32 board              |
| RDA5807FP     | FM receiver module           |
| Button        | Connected to GPIO25          |
| Speaker       | Connected to audio output of RDA5807FP |
| Optional Pull-down Resistor | If not using INPUT_PULLDOWN |

### 🧭 Pin Connections (ESP32)

| RDA5807FP Pin | ESP32 Pin | Description        |
|---------------|-----------|--------------------|
| SDA           | GPIO21    | I2C Data           |
| SCL           | GPIO22    | I2C Clock          |
| VCC           | 3.3V      | Power              |
| GND           | GND       | Ground             |
| Audio Out     | External amplifier/speaker |

## 📶 Predefined FM Channels

These values are written directly to the frequency register:
| Frequency (MHz) | Register Value (Hex) |
|------------------|----------------------|
| 88.8             | `0x490`              |
| 90.4             | `0x890`              |
| 94.4             | `0x1290`             |
| 103.0            | `0x2810`             |

## 📦 Setup

```cpp
Wire.begin(21, 22); // SDA, SCL pins for ESP32
```

## 🧩 Register 0x02 – Power and Control Register
| Bit(s) | Name             | Description                                          |
| ------ | ---------------- | ---------------------------------------------------- |
| 15     | `DHIZ`           | Audio output High-Z disable (0 = High-Z, 1 = normal) |
| 14     | `DMUTE`          | Mute disable (0 = mute, 1 = unmute)                  |
| 13     | `MONO`           | Force mono (0 = stereo, 1 = mono)                    |
| 12     | `BASS`           | Bass boost enable                                    |
| 11     | `NON_CALIBRATE`  | 0 = RCLK always supplied, 1 = not always             |
| 10     | `RCLK_DIRECT_IN` | Use external clock directly                          |
| 9      | `SEEKUP`         | 1 = Seek up, 0 = Seek down                           |
| 8      | `SEEK`           | Start seek (1 to initiate)                           |
| 7      | `SKMODE`         | Seek stop method                                     |
| 6:4    | `CLK_MODE`       | Clock mode select                                    |
| 3      | `RDS_EN`         | Enable RDS                                           |
| 2      | `NEW_METHOD`     | Enable better sensitivity                            |
| 1      | `SOFT_RESET`     | Software reset                                       |
| 0      | `ENABLE`         | Power up the chip                                    |


➡ Enables chip, resets, unmutes, enables bass boost, etc.

| Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|----|----|----|----|----|----|---|---|---|---|---|---|---|---|---|---|
| Val | 1  | 1  | 1  | 1  | 0  | 0  | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 0 | 1 | <br>


Hexadecimal: 0xF009 <br>
Binary: 0b1111000000001001 <br>
In the code Binary is used. <br>

## Register 0x03 Channel and Tuning control register

| Bit(s) | Name          | Description                              |
| ------ | ------------- | ---------------------------------------- |
| 15:6   | `CHAN`        | Channel/frequency bits                   |
| 5      | `DIRECT_MODE` | Used for direct frequency control (test) |
| 4      | `TUNE`        | Start tuning (1 = tune)                  |
| 3:2    | `BAND`        | Band select                              |
| 1:0    | `SPACE`       | Channel spacing (e.g., 100kHz)           |

Channel Calculation:

Channel = (Desired Frequency - Start Frequency) / Space <br>
For example: <br>
- Desired Frequency = 90.4Mhz
- Start Frequency   = 87.0Mhz (In my country it is 87.0-108.0)
- Space is 100Khz <br>
Therefore, Channel = 90.4-87.0/0.1 (100kHz is 0.1Mhz) = 34 <br>
As Channel is 15:6 total 10 bit register, Channel in binary is = 0b0000100010 <br>


| Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|----|----|----|----|----|----|---|---|---|---|---|---|---|---|---|---|
| Val | 0  | 0  | 0  | 0  | 1  | 0  | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | <br>

Binary: 0b000000100010010000 <br>
Hexadecimal: 0x890 \\\ 90.4Mhz <br>

## 🔈 Register 0x04 – Audio and GPIO Configuration Register

| Bit(s) | Name           | Description                          |
| ------ | -------------- | ------------------------------------ |
| 15     | `RSVD`         | Reserved                             |
| 14     | `STCIEN`       | Seek/Tune Complete Interrupt enable  |
| 13:12  | `RSVD`         | Reserved                             |
| 11     | `DE`           | De-emphasis (0 = 75µs, 1 = 50µs)     |
| 10     | `RDS_FIFO_CLR` | Clear RDS FIFO                       |
| 9      | `SOFTMUTE_EN`  | Enable soft mute                     |
| 8      | `AFCD`         | AFC disable                          |
| 7      | `RSVD1`        | Reserved                             |
| 6      | `I2S_ENABLE`   | Enable I2S output                    | <br>
| 5-4    | `GPIO3`        | GPIO3 function control               |
| 3-2    | `GPIO2`        | GPIO2 function control               |
| 1-0    | `GPIO1`        | GPIO1 function control               |

| Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|----|----|----|----|----|----|---|---|---|---|---|---|---|---|---|---|
| Val | 0  | 0  | 0  | 0  | 1  | 0  | 0 | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 0 |

Binary: 0b0000001001000000 <br>
Hexadecimal: 0x240


## 🔊 Register 0x05 – Volume and Seek Configuration Register

| Bit(s) | Name            | Description           |
| ------ | --------------- | --------------------- |
| 15     | `INT_MODE`      | Interrupt mode        |
| 14-12  | `RSVD`          | Reserved              |
| 11:8   | `SEEKTH`        | Seek threshold        |
| 7:6    | `LNA_ICSEL_BIT` | LNA current selection |
| 5:4    | `RSVD`          | Reserved              |
| 3:0    | `VOLUME`        | Volume level (0–15)   |

| Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|----|----|----|----|----|----|---|---|---|---|---|---|---|---|---|---|
| Val | 1  | 0  | 0  | 0  | 1  | 0  | 0 | 0 | 1 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |

Binary: 0b1000100010001111 <br>
Hexadecimal: 0x888F

## 🎧 Register 0x06 – I2S and Audio Interface Register

| Bit(s) | Name           | Description                                |
| ------ | -------------- | ------------------------------------------ |
| 15     | `RSVD`         | Reserved                                   |
| 14-13  | `OPEN_MODE`    | Enables register read/write (11 = open)    |
| 12     | `I2S_MODE`     | I2S mode: 0 = master, 1 = slave            |
| 11     | `SW_LR`        | Word select: left/right                    |
| 10     | `SCLK_I_EDGE`  | I2S clock edge (internal)                  |
| 9      | `DATA_SIGNED`  | I2S data format (0 = unsigned, 1 = signed) |
| 8      | `WS_I_EDGE`    | Word select edge                           |
| 7-4    | `I2S_SW_CNT`   | I2S sample width or delay                  |
| 3      | `SW_O_EDGE`    | If 1, invert ws output when master         |
| 2      | `SCLK_O_EDGE`  | If 1, invert sclk output when master       |
| 1      | `L_DELY`       | L-channel delay                            |
| 0      | `R_DELY`       | R-channel delay                            |

| Bit | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|-----|----|----|----|----|----|----|---|---|---|---|---|---|---|---|---|---|
| Val | 0  | 0  | 0  | 0  | 1  | 0  | 1 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |

#### Important Note: For MAX98357A __DATA_SIGNED__ bit should be 1, which means signed 16-bit audio data. 

Binary: 0b0000101010000000 <br>
Hexadecimal: 0xA80 <br>

## Connection with esp32:
ESP32S-GPIO21(SDA) - RDA5807FP PIN 8 <br>
ESP32S-GPIO22(SCL) - RDA5807FP PIN 7 <br>

MAX98357(Module)-LRC  -  RDA5807FP-GPIO1 <br>
MAX98357(Module)-DIN  -  RDA5807FP-GPIO2 <br>
MAX98357(Module)-BCLK -  RDA5807FP-GPIO3 <br>

MAX98357-VCC -ESP32-VIN or 5V
MAX98357-VCC -ESP32-GND

RDA5807FP-VCC/VDD-PIN-10 - ESP32-3.3V
RDA5807FP-GND-PIN-2,3,5,6,11,14 - ESP32-GND

For Antenne Use 75cm single core wire.

### ⚠️ I2C Line pullup resistor is needed. It is must. I used 10K.
```c++
#define RDAW 0x11  // 0b0010000
#define RDAR 0x10  //Confusing for ma Not used -> As it increment by one every time we accesse the register.
```
It also had an i2c address of 0x60. 
### IF anyone know information about this 0x60 address and how to use it, please let me know.

Writing the register to RDA5807FP 
```c++

void writeRegister(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(RDAW);
  Wire.write(reg);
  Wire.write(highByte(value));
  Wire.write(lowByte(value));
  Wire.endTransmission();
}

```

### This code is for testing purpose. For general usage, use library.
Library link: https://github.com/pu2clr/RDA5807 <br>
Code with this lib:

```c++

#include <RDA5807.h>

// I2C bus pin on ESP32
#define ESP32_I2C_SDA 21
#define ESP32_I2C_SCL 22

#define MAX_DELAY_RDS 40   // 40ms - polling method

long rds_elapsed = millis();

RDA5807 rx;

void showHelp()
{
  Serial.println("Type U to increase and D to decrease the frequency");
  Serial.println("Type S or s to seek station Up or Down");
  Serial.println("Type + or - to volume Up or Down");
  Serial.println("Type 0 to show current status");
  Serial.println("Type ? to this help.");
  Serial.println("==================================================");
  delay(1000);
}

// Show current frequency
void showStatus()
{
  char aux[80];
  sprintf(aux,"\nYou are tuned on %u MHz | RSSI: %3.3u dbUv | Vol: %2.2u | %s ",rx.getFrequency(), rx.getRssi(), rx.getVolume(), (rx.isStereo()) ? "Yes" : "No" );
  Serial.print(aux);
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) ;

    // The line below may be necessary to setup I2C pins on ESP32
    Wire.begin(ESP32_I2C_SDA, ESP32_I2C_SCL);
    
    rx.setup();
    rx.setMono(true); // Force stereo
    rx.setRDS(true);
    rx.setRdsFifo(true);
    rx.setI2SOn(true);
    rx.setI2SMaster(true);
    rx.setI2SSpeed(I2S_WS_STEP_32);
    rx.setI2SDataSigned(true);
    rx.setVolume(10);

    delay(500);

    // Select a station with RDS service in your place
    Serial.println("Channel 90.40 MHz");
    rx.setFrequency(9040); 
    // Enables SDR
    rx.setRDS(true);

    showHelp();
    showStatus();
 
}

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();
    switch (key)
    {
    case '+':
      rx.setVolumeUp();
      break;
    case '-':
      rx.setVolumeDown();
      break;
    case 'U':
    case 'u':
      rx.setFrequencyUp();
      break;
    case 'D':
    case 'd':
      rx.setFrequencyDown();
      break;
    case 'S':
      rx.seek(RDA_SEEK_WRAP, RDA_SEEK_UP);
      break;
    case 's':
      rx.seek(RDA_SEEK_WRAP, RDA_SEEK_DOWN);
      break;
    case '0':
      showStatus();
      break;
    case '?':
      showHelp();
      break;
    default:
      break;
    }
    delay(200);
    showStatus();
  } 
  delay(5);
}


```

#
# Thank you for reading.