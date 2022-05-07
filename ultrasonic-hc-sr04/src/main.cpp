/*
 * Goran Todorovic
 * dejakju@gmail.com
 * 
 * Ultrasonic Sensor HC-SR04
 */
#include <Arduino.h>

const int PIN_TRIG = 0x4;
const int PIN_ECHO = 0x2;

const int LED_R = 0x8;
const int LED_G = 0x9;
const int LED_B = 0xA;

float duration = 0.0f;
float distance = 0.0f;

float MicrosecondsToCentimeters(float duration);

void setup()
{
    Serial.begin(57600);
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
}

void loop()
{
    // initiating the trigger sequence (10ms pulse)
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    // Measure the response from the HC-SR04 Echo Pin
    duration = pulseIn(PIN_ECHO, HIGH);

    // Determine distance from duration
    // Use 343 metres per second as speed of sound
    distance = MicrosecondsToCentimeters(duration);

    // Print result to Serial Monitor
    Serial.print("Distance : ");
    Serial.println(distance);

    // Switch LEDs
    if (distance >= 15.0f)
    {
        digitalWrite(LED_B, HIGH);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_R, LOW);
    }
    else if (distance < 15.0f && distance >= 5.0f)
    {
        digitalWrite(LED_B, LOW);
        digitalWrite(LED_G, HIGH);
        digitalWrite(LED_R, LOW);
    }
    else if (distance < 5.0f)
    {
        digitalWrite(LED_B, LOW);
        digitalWrite(LED_G, LOW);
        digitalWrite(LED_R, HIGH);
    }

    // wait a little to calibrate
    delay(500);
}

float MicrosecondsToCentimeters(float duration)
{
    return ((duration / 2) * 0.0343f);
}
