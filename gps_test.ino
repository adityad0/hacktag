#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(3, 4);  // GSP_TX->3, GSP_RX->4
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

/*
  * PARSING:
  * $GPRMC provides time, date, latitude, longitude, altitude, and estimated velocity.
  * $GPGGA sentence provides essential fix data which provides the 3D location and accuracy data.
*/

void loop() {
  while(gpsSerial.available() > 0) {
    // Serial.write(gpsSerial.read());
    if(gps.encode(gpsSerial.read())) {
      displayGPSinfo();
    }
  }

  if(millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    delay(1000);
    while(true);
  }
}

void displayGPSinfo() {
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
    if(gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
    Serial.print(":");
    if(gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
    Serial.print(":");
    if(gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
    Serial.print(".");
    if(gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.println(gps.time.centisecond());
  } else {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}