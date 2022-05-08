#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define EEPROM_I2C_ADDRESS 0x50 // I2C ADDRESS = B01010000

const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal Lcd(rs, en, d4, d5, d6, d7);

void writeEEPROM(int address, byte data, int i2c_address)
{
    Wire.beginTransmission(i2c_address);
    Wire.write((int)(address & 0xFF));
    Wire.write(data);
    Wire.endTransmission();
    delay(5);
}

byte readEEPROM(int address, int i2c_address)
{
    byte data = 0xFF;
    Wire.beginTransmission(i2c_address);
    Wire.write((int)(address & 0xFF));
    Wire.endTransmission();
    Wire.requestFrom(i2c_address, 1);
    while (Wire.available())
    {
        data = Wire.read();
    }
    return data;
}

void setup()
{
    Serial.begin(57600);
    Wire.begin();
    Lcd.begin(16, 2);

    int value = 0xFF;

    for (int address = 0x00; address <= 0xFF; address++)
    {
        Lcd.clear();
        Lcd.setCursor(0,0);
        Lcd.print("Writing ");
        Lcd.print(value);
        Lcd.setCursor(0,1);
        Lcd.print("to ");
        Lcd.print(address);
        writeEEPROM(address, value--, EEPROM_I2C_ADDRESS);
        delay(20);
    }

    delay(5000);
    Lcd.clear();
    delay(2000);

    for (int address = 0; address < 0x100; address++)
    {
        Lcd.clear();
        Lcd.setCursor(0,0);
        Lcd.print("Reading ");
        Lcd.print(readEEPROM(address, EEPROM_I2C_ADDRESS));
        Lcd.setCursor(0,1);
        Lcd.print("from ");
        Lcd.print(address);
        delay(20);
    }

}

void loop()
{
}