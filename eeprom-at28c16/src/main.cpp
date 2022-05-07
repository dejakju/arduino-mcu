/*
 * Goran Todorovic
 * dejakju@gmail.com
 * 
 * AT28C16 EEPROM Programmer
 */
#include <Arduino.h>

const int SHIFT_DATA = 2;
const int SHIFT_LATCH = 3;
const int SHIFT_CLK = 4;

const int D0 = 5;
const int D1 = 6;
const int D2 = 7;
const int D3 = 8;
const int D4 = 9;
const int D5 = 10;
const int D6 = 11;
const int D7 = 12;

const int CE = 14;
const int OE = 15;
const int WE = 16;

void set_CE(byte state) { digitalWrite(CE, state); }
void set_OE(byte state) { digitalWrite(OE, state); }
void set_WE(byte state) { digitalWrite(WE, state); }

void set_BusInput()
{
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
}

void set_BusOutput()
{
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
}

void set_Address(int address)
{
    digitalWrite(SHIFT_LATCH, LOW);
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8));
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
    digitalWrite(SHIFT_LATCH, HIGH);
}

byte read_EEPROM(int address)
{
    set_CE(HIGH);
    set_OE(HIGH);
    set_WE(HIGH);

    set_BusInput();
    set_Address(address);

    set_CE(LOW);
    set_OE(LOW);

    return ((digitalRead(D7) << 7) +
            (digitalRead(D6) << 6) +
            (digitalRead(D5) << 5) +
            (digitalRead(D4) << 4) +
            (digitalRead(D3) << 3) +
            (digitalRead(D2) << 2) +
            (digitalRead(D1) << 1) +
            digitalRead(D0));

    set_OE(HIGH);
    set_CE(HIGH);
}

void write_EEPROM(int address, byte data)
{
    set_CE(HIGH);
    set_OE(HIGH);
    set_WE(HIGH);

    set_BusOutput();
    set_Address(address);

    digitalWrite(D7, (data >> 7) & 1);
    digitalWrite(D6, (data >> 6) & 1);
    digitalWrite(D5, (data >> 5) & 1);
    digitalWrite(D4, (data >> 4) & 1);
    digitalWrite(D3, (data >> 3) & 1);
    digitalWrite(D2, (data >> 2) & 1);
    digitalWrite(D1, (data >> 1) & 1);
    digitalWrite(D0, (data) & 1);

    set_CE(LOW);
    set_WE(LOW);
    delayMicroseconds(10);

    set_WE(HIGH);
    set_CE(HIGH);

    delay(10);
}

void read_Contents()
{
    for (int base = 0; base < 0x800; base += 16)
    {
        byte data[16];
        for (int offset = 0; offset < 16; offset++)
        {
            data[offset] = read_EEPROM(base + offset);
        }
        
        char outBuffer[92];
        
        sprintf(outBuffer, "%08X   %02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X   %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
        base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
                data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15],
                ((data[0] >= 0x20 && data[0] <= 0x7F) ? (char)data[0] : '.'),
                ((data[1] >= 0x20 && data[1] <= 0x7F) ? (char)data[1] : '.'),
                ((data[2] >= 0x20 && data[2] <= 0x7F) ? (char)data[2] : '.'),
                ((data[3] >= 0x20 && data[3] <= 0x7F) ? (char)data[3] : '.'),
                ((data[4] >= 0x20 && data[4] <= 0x7F) ? (char)data[4] : '.'),
                ((data[5] >= 0x20 && data[5] <= 0x7F) ? (char)data[5] : '.'),
                ((data[6] >= 0x20 && data[6] <= 0x7F) ? (char)data[6] : '.'),
                ((data[7] >= 0x20 && data[7] <= 0x7F) ? (char)data[7] : '.'),
                ((data[8] >= 0x20 && data[8] <= 0x7F) ? (char)data[8] : '.'),
                ((data[9] >= 0x20 && data[9] <= 0x7F) ? (char)data[9] : '.'),
                ((data[10] >= 0x20 && data[10] <= 0x7F) ? (char)data[10] : '.'),
                ((data[11] >= 0x20 && data[11] <= 0x7F) ? (char)data[11] : '.'),
                ((data[12] >= 0x20 && data[12] <= 0x7F) ? (char)data[12] : '.'),
                ((data[13] >= 0x20 && data[13] <= 0x7F) ? (char)data[13] : '.'),
                ((data[14] >= 0x20 && data[14] <= 0x7F) ? (char)data[14] : '.'),
                ((data[15] >= 0x20 && data[15] <= 0x7F) ? (char)data[15] : '.'));

        if (base % 256 == 0)
        {
          Serial.println(" OFFSET     0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F         ASCII     ");
        }

        Serial.println(outBuffer);
    }
}

void erase_EEPROM()
{
    Serial.print("Erasing EEPROM");
    unsigned long starttime = millis();
    byte data = 0xFF;
    for (int address = 0x00; address < 0x0800; address++)
    {
        write_EEPROM(address, data);

        if (address % 32 == 0)
        {
          Serial.print(".");
        }
    }
    unsigned long endtime = millis();
    Serial.print(" done. (time elapsed = ");
    Serial.print((endtime - starttime), DEC);
    Serial.println("ms)");
}

void write_Test()
{
    byte data = 0x00;
    Serial.print("Start WRITING");
    unsigned long starttime = millis();
    // write just to the first 256 bytes...
    for (unsigned int address = 0x00; address < 0x0100; address++)
    {
        write_EEPROM(address, data++);

        if (address % 32 == 0)
        {
          Serial.print(".");
        }
    }
    unsigned long endtime = millis();
    Serial.print(" done. (time elapsed = ");
    Serial.print((endtime - starttime), DEC);
    Serial.println("ms)");
}

void setup()
{
    pinMode(SHIFT_DATA, OUTPUT);
    pinMode(SHIFT_LATCH, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);

    digitalWrite(CE, HIGH);
    pinMode(CE, OUTPUT);
    digitalWrite(OE, HIGH);
    pinMode(OE, OUTPUT);
    digitalWrite(WE, HIGH);
    pinMode(WE, OUTPUT);

    Serial.begin(57600);
    delay(1000);


    // Erase the entire EEPROM
    // erase_EEPROM();

    // Write some test data
    // write_Test();

    // write_EEPROM(0x7D2, 0x11);
    // write_EEPROM(0x7DD, 0x11);
    // write_EEPROM(0x7E1, 0x11);
    // write_EEPROM(0x7EE, 0x11);

    // Print the contents of the EEPROM to the Serial Monitor
    read_Contents();

}

void loop()
{
}