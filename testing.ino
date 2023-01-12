#include <SoftwareSerial.h>
SoftwareSerial gsmSerial(2, 1);  // GSM_TX->2, GSM_RX->1

// char* sendATcommand(const char* ATcommand);

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  Serial.println("GSM Initialized.");

  delay(2000);

  char gsmMsg[100];
  int gsmLen = sendATcommand("AT+CPIN?", gsmMsg);
}

void loop() {
  // while(Serial.available())
  //   gsmSerial.write(Serial.read());
  // while(gsmSerial.available())
  //   Serial.write(gsmSerial.read());
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