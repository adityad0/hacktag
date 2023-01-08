/* Author: @adityad0
* HACKTAG https://github.com/adityad0/hacktag/
* License: https://github.com/adityad0/hacktag/LICENSE.md
* GPS tracker with GSM SMS alert and BLE */

// Include libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Declare global variables
char ALERT_PHONE_NUMBER[50] = "+12065551234";
char SELF_PHONE_NUMBER[20] = "+12065551234";
int GPS_BAUD = 9600;
int GSM_BAUD = 9600;

// Declare global objects
TinyGPSPlus gps;
SoftwareSerial GSM(2, 3); // RX, TX
SoftwareSerial GPS(4, 5); // RX, TX

// Declare functions
void sendSMS(char *number, char *message);

void setup() {
    Serial.begin(9600);
    Serial.println("= = = = HACKTAG = = = =\n");
    GSM.begin(GSM_BAUD);
    Serial.println("GSM Initialized.");
    GPS.begin(GPS_BAUD);
    Serial.println("GPS Initialized.");
}

void loop() {
    // This sketch displays information every time a new sentence is correctly encoded.
    while (gpsSerial.available() > 0)
        if (gps.encode(gpsSerial.read()))
            displayInfo();

    // If 5000 milliseconds pass and there are no characters coming in
    // over the software serial port, show a "No GPS detected" error
    if(millis() > 5000 && gps.charsProcessed() < 10) {
        Serial.println("No GPS detected");
        while(true);
    }
}

void displayInfo() {
    if(gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
    } else {
        Serial.println("Location: Not Available");
    }

    Serial.print("Date: ");
    if(gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
    } else {
        Serial.println("Not Available");
    }

    Serial.print("Time: ");
    if(gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
            Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10) Serial.print(F("0"));
            Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10) Serial.print(F("0"));
            Serial.print(gps.time.second());
        Serial.print(".");
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
            Serial.println(gps.time.centisecond());
    } else {
        Serial.println("Not Available");
    }

    Serial.println();
    Serial.println();
    delay(1000);
}
