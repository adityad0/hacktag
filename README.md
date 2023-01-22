# HackTag

A home made GPS tracker with live tracking tracking updates using GPS and GSM connectivity.
This project uses a SIM A7670C 4G/LTE Module, NEO 6M GPS Module and a SEEED XIAO SAMD 21 Microcontroller to fetch the current location using GPS and send out location updates over SMS at a fixed intervals or whenever the device moves a certain distance from it's last known position. Currently, it does not support UWB like the Apple Airtag or other devices. The device cannot send location updates without a 2G/4G mobile network.

## What does the program do?
The program uses the TinyGPS++ and SoftwareSerial library to fetch the location from the GPS module every 1 second. The location data is processed using the TinyGPS++ library. If the device is moving more that 10 meters/second, an SMS alert is sent to the predefined mobile number. However, no SMS alert is sent if the device is moving under 10 meters/second. This issue will be fixed in the next version where an additional alert will be sent if the device moves more than 10 meters from it's initial position. In addition to this, an SMS alert is also sent every hour containing the Latitude, Longitude, Altitude and the current Date and Time with a Google Maps link with a dropped pin at the coordinates.

## Components
1. GPS Module - NEO 6M GPS Module from u-blox or another compatible type.
2. Microcontroller - Arduino, SEEED XIAO ESP32, RP2040, nRF52840 or another compatible type.
3. GSM Module - SIM800L, SIM A7670C another compatible type (Note: SIM800L supports only 2G networks whereas the A7670C supports both 4G/LTE and 2G GSM).
4. Li-ion Battery (3.7v) (Optional).
5. Li-ion charge module.

## Getting started
1. Download the hacktag folder and save it to a known location.
2. Open the Arduino IDE and open this downloaded folder.
3. Connect your Arduino/SEEED Xiao board to your computer.
4. Select your board and port under the Tools option. (On Windows, use device manager to find the port)
5. Click on upload.
6. Make the connections as given in the program. Note that TX must be connected to RX and vice versa. In my case, the GSM module had incorrect labeling and I had to connect TX to TX and RX to RX. Please check this before making the connections.
7. Reset your board and open Serial Monitor or Putty for the port with a baud rate of 9600.
Done!


## Future updates
1. Currently there is no plan for UWB support. If you wish to implement something similar to this, I recommend taking a look at this repository. https://github.com/0015/ThatProject/tree/master/ESP32_UWB
2. I'm currently working on a simple Android app using app inventor. When complete, I will release all the files here so all users can recreate the app for their own use. If you wish to use Wi-Fi or Bluetooth, I recommend that you use the SEEED XIAO nRF52840 or the ESP32. You can also use a module such as the HC-05 bluetooth module for app connectivity.


**NOTE:** This project is for educational purposes only and is provided as-is without any warranty whatsoever. Please follow all local laws concerning such trackers before getting started. The creator of this project, the contributors, authors or anyone involved in the making of this project are not liable or responsible for any damages of any kind whatsoever.


Full License: https://creativecommons.org/licenses/by-sa/4.0/legalcode


HackTag © 2023 by Aditya Desai is licensed under Attribution-ShareAlike 4.0 International

![GitHub](https://img.shields.io/github/license/adityad0/hacktag?style=flat-square)
![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/adityad0/hacktag?style=flat-square)
![GitHub issues](https://img.shields.io/github/issues-raw/adityad0/hacktag?style=flat-square)
![GitHub commit activity](https://img.shields.io/github/commit-activity/m/adityad0/hacktag?style=flat-square)
![Twitter Follow](https://img.shields.io/twitter/follow/adityadesaig?style=social)
![GitHub followers](https://img.shields.io/github/followers/adityad0?style=social)
![GitHub forks](https://img.shields.io/github/forks/adityad0/hacktag?style=social)
![GitHub User's stars](https://img.shields.io/github/stars/adityad0?style=social)
