#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 Rfid(SS_PIN, RST_PIN);

void printHex(byte *buffer, byte bufferSize);

void setup()
{
    Serial.begin(57600);
    SPI.begin();
    Rfid.PCD_Init();
}

void loop()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!Rfid.PICC_IsNewCardPresent())
        return;

    // Verify if the NUID has been readed
    if (!Rfid.PICC_ReadCardSerial())
        return;

    Serial.print(F("RFID Tag UID:"));
    printHex(Rfid.uid.uidByte, Rfid.uid.size);
    Serial.println("");

    Rfid.PICC_HaltA(); // Halt PICC
    delay(1000);
}

void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
