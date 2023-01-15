/*
  * Author: @adityad0 [ https://github.com/adityad0/ ]
  * HACKTAG https://github.com/adityad0/hacktag/
  * License: https://github.com/adityad0/hacktag/LICENSE.md
  * GPS tracker with GSM SMS alert and BLE
*/

/* This program uses two function to get the current GPS corrdinates, date and time and prints it to the serial. */

// Include libraries
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Declare functions
void get_gps_pos(double* coords);
void get_gps_dttm(int *dttm);
void get_coord_distance(double *coords_A, double *coords_B, double *distance_moved);

// Declare global variables
double coords[3] = {0.0, 0.0, 0.0}; // Default GPS coordinates
int dttm[6] = {0, 0, 0, 0, 0, 0}; // Latest GPS date time in Year, Month, Day, Hour, Minute, Seconds format
double distance_moved = 0.00; // Distance moved in meters
double initial_coords[3] = {0.0, 0.0, 0.0}; // Initial GPS coordinates
int sms_interval = 60; // Time between each update in minutes
int last_sms_dttm[6] = {0, 0, 0, 0, 0, 0}; // Time of last SMS update

// Declare global objects
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
  while(gpsSerial.available() > 0) {
    // Serial.write(gpsSerial.read());
    // if(gps.encode(gpsSerial.read())) {
    //   displayGPSinfo();
    // }
    if(gps.encode(gpsSerial.read())) {
      get_gps_pos(coords);
      get_gps_dttm(dttm);

      display_gps_data();

      initial_coords[0] = coords[0];
      initial_coords[1] = coords[1];
      initial_coords[2] = coords[2];

      get_coord_distance(initial_coords, coords, &distance_moved);
      Serial.print("Distance moved: ");
      Serial.println(distance_moved);
    }
  }

  if(millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    delay(1000);
    while(true);
  }
}

void get_gps_pos(double* coords) {
  double *coords_ptr = coords;
  if(gps.location.isValid()) {
    *coords_ptr = gps.location.lat();
    coords_ptr++;
    *coords_ptr = gps.location.lng();
    coords_ptr++;
    *coords_ptr = gps.altitude.meters();
    coords_ptr++;
  } else {
    *coords_ptr = 0.0;
    coords_ptr++;
    *coords_ptr = 0.0;
    coords_ptr++;
    *coords_ptr = 0.0;
    coords_ptr++;
  }
}

void get_gps_dttm(int* dttm) {
  int *dttm_ptr = dttm;
  if(gps.date.isValid()) {
    dttm_ptr[0] = gps.date.year();
    dttm_ptr[1] = gps.date.month();
    dttm_ptr[2] = gps.date.day();
  } else {
    dttm_ptr[0] = 0;
    dttm_ptr[1] = 0;
    dttm_ptr[2] = 0;
  }
  if(gps.time.isValid()) {
    dttm_ptr[3] = gps.time.hour();
    dttm_ptr[4] = gps.time.minute();
    dttm_ptr[5] = gps.time.second();
  } else {
    dttm_ptr[3] = 0;
    dttm_ptr[4] = 0;
    dttm_ptr[5] = 0;
  }
}

void get_coord_distance(double* coords_A, double* coords_B, double* distance_moved) {
  double *ptr_A = coords_A;
  double *ptr_B = coords_B;
  double *ptr_distance_moved = distance_moved;

  double lat_A = *ptr_A;
  ptr_A++;
  double lon_A = *ptr_A;
  ptr_A++;
  double lat_B = *ptr_B;
  ptr_B++;
  double lon_B = *ptr_B;
  ptr_B++;

  *ptr_distance_moved = TinyGPSPlus::distanceBetween(lat_A, lon_A, lat_B, lon_B);
}

void display_gps_data() {
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
  Serial.println("\n\n");
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
    if (gps.time.hour() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10)
      Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  } else {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
