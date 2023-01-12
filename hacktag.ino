/*
  * Author: @adityad0
  * HACKTAG https://github.com/adityad0/hacktag/
  * License: https://github.com/adityad0/hacktag/LICENSE.md
  * GPS tracker with GSM SMS alert and BLE
*/

// Include libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Declare functions
int sendATcommand(const char* ATcommand, char* gsmMsg);
void sendSMS(char *number, char *message);
float *getCoords();

// Declare global variables
char ALERT_PHONE_NUMBER[20] = "+12065551234";
char SELF_PHONE_NUMBER[20] = "+12065551234";
int GPS_BAUD = 9600;
int GSM_BAUD = 9600;
char gsmMsg[100];

// Declare global objects
TinyGPSPlus gps;
SoftwareSerial gsmSerial(2, 1);  // GSM_TX->2, GSM_RX->1
SoftwareSerial gpsSerial(3, 4);  // GSP_TX->3, GSP_RX->4

void setup() {
  Serial.begin(9600);
  Serial.println("= = = = HACKTAG = = = =\n");
  gsmSerial.begin(GSM_BAUD);
  Serial.println("GSM Initialized.");
  gpsSerial.begin(GPS_BAUD);
  Serial.println("GPS Initialized.");
  delay(2000);
  sendATcommand("AT+CPIN?", gsmMsg);
  Serial.println(gsmMsg[0]);
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while(gpsSerial.available() > 0)
    if(gps.encode(gpsSerial.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true)
      ;
  }
}

void displayInfo() {
  if (gps.location.isValid()) {
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
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  } else {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid()) {
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

void sendSMS(char *number, char *message) {
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(number);
  gsmSerial.println("\"");
  delay(1000);
  gsmSerial.println(message);
  delay(1000);
  gsmSerial.println((char)26);  // ASCII code of CTRL+Z
  delay(1000);
  gsmSerial.println();
  delay(1000);
}

int sendATcommand(const char* ATcommand, char* gsmMsg) {
  char *ptr = gsmMsg;
  gsmSerial.println(ATcommand);
  delay(500);
  while(gsmSerial.available() > 0) {
    *ptr = gsmSerial.read();
    ptr++;
  }
  return ptr - gsmMsg;
}
