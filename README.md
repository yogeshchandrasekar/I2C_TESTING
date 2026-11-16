# I2C EEPROM Interfacing with LPC2124 (24C04A + 16x2 LCD)

A simple embedded C project that writes a string to a **24C04A I2C EEPROM**, reads it back, and shows it on a **16x2 LCD**. Built for the **LPC21xx (ARM7)** family and simulated in **Proteus**.

## What it does

1. Initializes the LCD and I2C0 peripheral.
2. Displays `I2C TESTING` for 2 seconds.
3. Writes `WORKING` (7 bytes) to EEPROM address `0x00` using a page write.
4. Reads the 7 bytes back using a sequential read.
5. Clears the LCD and displays the data read from the EEPROM.

If the I2C works, the LCD finally shows: `WORKING`

## Hardware

| Component | Details |
|-----------|---------|
| MCU | LPC2124 (LPC21xx series) |
| EEPROM | AT24C04A (512 bytes, 16-byte page) |
| Display | LM016L (16x2 LCD, 8-bit mode) |
| Pull-ups | 2 x 4.7k to 3.3V (one on SDA, one on SCL) |

## Pin connections

### I2C (EEPROM)

| LPC2124 pin | Signal | EEPROM pin |
|-------------|--------|------------|
| P0.2 (pin 22) | SCL0 | SCL (pin 6) |
| P0.3 (pin 26) | SDA0 | SDA (pin 5) |

- EEPROM **A1, A2, WP, GND** go to GND. **VCC** goes to 3.3V.
- A0 is not used inside the 24C04A.
- Put a **4.7k pull-up to 3.3V** on both SDA and SCL.
- EEPROM 7-bit address: `0x50` (`0x50` covers bytes 0-255, `0x51` covers bytes 256-511).

### LCD (8-bit mode, as defined in the code)

| LPC2124 pin | LCD pin |
|-------------|---------|
| P0.17 - P0.24 | D0 - D7 |
| P0.5 | RS |
| P0.6 | E |
| GND | RW, VSS |
| 5V / 3.3V | VDD |

Make sure the wires in your schematic match these pins, because the `#define`s in `lcd.h` decide which pins are used.

## File structure

| File | Purpose |
|------|---------|
| `main.c` | Test program: write, read back, display |
| `lcd.h` | LCD driver (`LCD_INIT`, `LCD_COMMAND`, `LCD_DATA`, `LCD_STRING`, `LCD_INT`) |
| `I2C.H` | I2C0 driver (`I2C_INIT`, `I2C_START`, `I2C_RESTART`, `I2C_WRITE`, `I2C_STOP`, `I2C_NACK`, `i2c_masterack`) |
| `EEPROM.H` | EEPROM functions (byte write/read, page write, sequential read) |
| `delay.h` | `delay_ms()` using Timer0 |

## API

```c
void i2c_eeprom_write(unsigned char slave, unsigned char addr, unsigned char data);
unsigned char i2c_eeprom_read(unsigned char slave, unsigned char addr);
void i2c_page_write(unsigned char slave, unsigned char addr, unsigned char *p, unsigned int n);
void i2c_eeprom_seq_read(unsigned char slave, unsigned char addr, unsigned char *p, unsigned char n);
```

## Clock assumption

`delay_ms()` and the I2C bit rate assume **PCLK = 15 MHz**:

- Timer0: `T0PR = 15000 - 1` gives a 1 ms tick.
- I2C: `I2SCLH = I2SCLL = 75` gives about 100 kHz.

If your PCLK is different, adjust these values. In Proteus, also set the LPC2124 clock frequency in its properties.

## Recommended fixes

- Use `while(1);` at the end of `main()`.
- Initialize the LCD with `0x38` first:
  ```c
  delay_ms(20);
  LCD_COMMAND(0x38);
  LCD_COMMAND(0x0C);
  LCD_COMMAND(0x06);
  LCD_COMMAND(0x01);
  ```
- Wrap macros in parentheses: `#define LCD_D (0xFF<<17)`.
- Cast string literals: `(unsigned char*)"WORKING"`.

## Troubleshooting

| Symptom | Likely cause |
|---------|--------------|
| Program hangs after "I2C TESTING" | Missing pull-ups, SCL/SDA on the wrong pins, or bus stuck |
| Status `0x08` after START | START works |
| Status `0x18` after address | EEPROM ACKed, wiring is good |
| Status `0x20` after address | No ACK: check A1/A2, power, address |
| LCD shows `0xFF` or garbage | WP pin is high or the write did not complete |
| First characters missing on LCD | Wrong LCD init order or no power-up delay |

**Proteus tip:** add the **I2C Debugger** instrument on SDA/SCL to see every START, address and data byte on the bus.

## Tools

- Keil uVision (ARM7 / LPC21xx)
- Proteus (simulation)

## Author

Your name here
