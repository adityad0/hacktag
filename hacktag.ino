/*
  * Author: @adityad0 [https://github.com/adityad0/]
  * HACKTAG https://github.com/adityad0/hacktag/
  * License: https://github.com/adityad0/hacktag/LICENSE.md
  * GPS tracker with GSM SMS alert and BLE
*/


/* Include libraries */
#include <TinyGPS++.h>  // Library to parse and collect GPS information
#include <SoftwareSerial.h> // Library to use software serial communication

/* Declare global variables */
  #define GPS_BAUD 9600 // Baud rate for communication between the microcontroller and the GPS module
  #define GSM_BAUD 9600 // Baud rate for communication between the microcontroller and the GSM module
  #define SERIAL_BAUD 9600 // Baud rate for communication between the microcontroller and the computer
  char ALERT_PHONE_NUMBER[20] = "+12065551234"; // The phone number which will receive SMS alerts
  char SELF_PHONE_NUMBER[20] = "+12065551234"; // The phone number associated with the SIM card present in the GSM module
  // Variable declerations for the GSM module
  char gsmMsg[100]; // Variable to store the response of the GSM module
  // Variable declerations for the GPS module
  float coords[3] = {0.0, 0.0, 0.0}; // Default GPS coordinates
  int dttm[6] = {0, 0, 0, 0, 0, 0}; // Latest GPS date time in Year, Month, Day, Hour, Minute, Seconds format
  float distance_moved = 0.00; // Distance moved in meters
  float initial_coords[3] = {0.0, 0.0, 0.0}; // Initial GPS coordinates
  int last_sms_dttm[6] = {0, 0, 0, 0, 0, 0}; // Time of last SMS update
  #define sms_interval 60 // Time between each update in minutes
  #define move_distance 100 // Distance moved in meters before sending an SMS alert

// Declare global objects
TinyGPSPlus gps;
SoftwareSerial gsmSerial(9, 10); // TX of GSM module to pin 10, RX of GSM module to pin 9
SoftwareSerial gpsSerial(3, 4); // TX of GPS module to pin 3, RX of GPS module to pin 4

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);
  Serial.println("= = = = HACKTAG = = = =");
  delay(500);
  gsmSerial.begin(GSM_BAUD);
  Serial.println("GSM Initialized.");
  delay(500);
  gpsSerial.begin(GPS_BAUD);
  Serial.println("GPS Initialized.");

  delay(1000);

  int at_resp_len = sendATcommand("AT+CPIN?", gsmMsg);
  Serial.println(gsmMsg);

  // sendSMS("Hello, world!");
  sendAlert();
}

void loop() {
  // while(Serial.available()) {
  //   gsmSerial.println(Serial.read());
  // }
  // while(gsmSerial.available()) {
  //   Serial.println(gsmSerial.read());
  // }
  // while(gpsSerial.available() > 0) {
  //   if(gps.encode(gpsSerial.read())) {
  //     get_gps_pos(coords);
  //     get_gps_dttm(dttm);
  //     // display_gps_data();
  //     initial_coords[0] = coords[0]; initial_coords[1] = coords[1]; initial_coords[2] = coords[2];
  //     get_coord_distance(initial_coords, coords, &distance_moved);
  //     if(distance_moved >= move_distance) {
  //       sendSMS("Alert: device moved! Date & Time: " + char(dttm[0]) + '-' + char(dttm[1]) + '-' + char(dttm[2]) + ' ' + char(dttm[3]) + ':' + char(dttm[4]) + ':' + char(dttm[5]) + ' Lat: ' + char(coords[0]) + ' Long: ' + char(coords[1]) + ' Alt: ' + char(coords[2]) + 'm');
  //     }
  //   }
  // }
  // if(millis() > 5000 && gps.charsProcessed() < 10) {
  //   Serial.println("No GPS detected");
  //   delay(1000);
  //   while(true);
  // }
}

/* Functions to interact with the GPS module */
void get_gps_pos(float* coords) {
  float *coords_ptr = coords;
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

float get_coord_distance(float lat1, float lon1, float lat2, float lon2) {
  TinyGPSPlus gps;
  return gps.distanceBetween(lat1, lon1, lat2, lon2);
}

void display_gps_data() {
  Serial.print("LAT: ");
  Serial.print(coords[0]);
  Serial.print(" LON: ");
  Serial.print(coords[1]);
  Serial.print(" ALT: ");
  Serial.println(coords[2]);
  // Serial.print("Date: ");
  // Serial.print(dttm[0]);
  // Serial.print("-");
  // Serial.print(dttm[1]);
  // Serial.print("-");
  // Serial.println(dttm[2]);
  // Serial.print("Time: ");
  // Serial.print(dttm[3]);
  // Serial.print(":");
  // Serial.print(dttm[4]);
  // Serial.print(":");
  // Serial.println(dttm[5]);
  Serial.println("\n\n");
}

/* Functions to interact with the GSM module */
void sendSMS(char* message) {
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(500);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(ALERT_PHONE_NUMBER);
  gsmSerial.println("\"");
  delay(500);
  gsmSerial.println(message);
  // gsmSerial.println();
  gsmSerial.println((char)26);  // ASCII code of CTRL+Z
  delay(500);
  gsmSerial.println();
  Serial.println("SMS sent! content: ");
  Serial.println(message);
}

int sendATcommand(const char* ATcommand, char* gsmMsg) {
  char *ptr = gsmMsg;
  gsmSerial.write(ATcommand);
  delay(500);
  while(gsmSerial.available() > 0) {
    *ptr = gsmSerial.read();
    ptr++;
  }
  *ptr = '\0';
  return ptr - gsmMsg;
}

void sendAlert() {
  // float lat, lon, alt;
  // float *lat = &coords[0];
  // float *lon = &coords[1];
  // float *alt = &coords[2];
  float lat = 51.509865, lon = -0.118092, alt = 10.4; // Coordinates for test message
  String message = "ALERT: Current Location - Lat: " + String(lat) + ", Long: " + String(lon) + ", Alt: " + String(alt) + " on " + String(__DATE__) + " " + String(__TIME__) + "\n" + "Map Link: " + "https://www.google.com/maps/search/?api=1&query=" + String(lat) + "," + String(lon);
  sendSMS((char*)message.c_str());
}
