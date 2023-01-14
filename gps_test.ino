/*
  * Author: @adityad0 [ https://github.com/adityad0/ ]
  * HACKTAG https://github.com/adityad0/hacktag/
  * License: https://github.com/adityad0/hacktag/LICENSE.md
  * GPS tracker with GSM SMS alert and BLE
*/

/* This program uses two function to get the current GPS corrdinates, date and time and prints it to the serial. */

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(3, 4);  // GSP_TX->3, GSP_RX->4
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

/*
  * TO PARSE THE GPS DATA:
  * $GPRMC provides time, date, latitude, longitude, altitude, and estimated velocity.
  * $GPGGA sentence provides essential fix data which provides the 3D location and accuracy data.
*/

void loop() {
  double coords[3] = {0.0, 0.0, 0.0};
  int dttm[6] = {0, 0, 0, 0, 0, 0};
  while(gpsSerial.available() > 0) {
    if(gps.encode(gpsSerial.read())) {
      get_gps_pos(coords);
      get_gps_dttm(dttm);
      Serial.print("Latitude: ");
      Serial.println(coords[0]);
      Serial.print("Longitude: ");
      Serial.println(coords[1]);
      Serial.print("Altitude: ");
      Serial.println(coords[2]);
      Serial.print("Date: ");
      Serial.print(dttm[0]);
      Serial.print("-");
      Serial.print(dttm[1]);
      Serial.print("-");
      Serial.println(dttm[2]);
      Serial.print("Time: ");
      Serial.print(dttm[3]);
      Serial.print(":");
      Serial.print(dttm[4]);
      Serial.print(":");
      Serial.println(dttm[5]);
    }
  }

  if(millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    delay(1000);
    while (true)
      ;
  }
}

void get_gps_pos(double* coords) {
  double *ptr = coords;
  if(gps.location.isValid()) {
    *ptr = gps.location.lat();
    ptr++;
    *ptr = gps.location.lng();
    ptr++;
    *ptr = gps.altitude.meters();
    ptr++;
  }
}

void get_gps_dttm(int *dttm) {
  if(gps.date.isValid()) {
    dttm[0] = gps.date.year();
    dttm[1] = gps.date.month();
    dttm[2] = gps.date.day();
  }
  if(gps.time.isValid()) {
    dttm[3] = gps.time.hour();
    dttm[4] = gps.time.minute();
    dttm[5] = gps.time.second();
  }
}
