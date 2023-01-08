# Micropython program for ESP, SEEED microcontrollers.
"""
ssd1306 library
https://github.com/micropython/micropython/blob/master/drivers/display/ssd1306.py
"""

# Libraries
from machine import I2C, Pin, UART
from time import sleep
from ssd1306 import SSD1306_I2C
import utime

# Variable Declarations
ALERT_PHONE_NUMBER = "+12065551234"
SELF_PHONE_NUMBER = "+12065551234";

uart1 = UART(1, 9600)

LED_INBUILT = Pin(25, Pin.OUT)
LED_RGB_R = Pin(22, Pin.OUT)
LED_RGB_G = Pin(21, Pin.OUT)
LED_RGB_B = Pin(20, Pin.OUT)

i2c0 = I2C(0, sda = Pin(0), scl = Pin(1), freq = 40000)
def get_i2c_devices(i2c0 = i2c0):
    print(i2c0)
    print("Available i2c devices: " + str(i2c0.scan()))

WIDTH = 128
HEIGHT = 64

oled = SSD1306_I2C(WIDTH, HEIGHT, i2c0)
sleep(1)

oled.fill(0)

oled.text("Initialized!", 0, 0)
oled.show()

def sendCMD_waitResp(cmd, uart = uart1, timeout = 2000):
    uart.write(cmd)
    resp = waitResp(uart, timeout)
    resp = resp[-1].decode().replace('\r', '').split('\n')
    return resp

def waitResp(uart = uart1, timeout = 2000):
    prvMills = utime.ticks_ms()
    resp = b""
    while(utime.ticks_ms() - prvMills) < timeout:
        if uart.any():
            resp = [resp, uart.read()]
            if resp != '':
                return resp

def get_gsm_operator():
    resp = sendCMD_waitResp("AT+CSPN?\r\n", uart1)
    sleep(4)
    if uart1.any():
        resp = [resp, uart1.read()]
    try:
        opr = resp[1].decode().replace('\r', '').split('\n')
        opr = opr[1].split('"')
        opr = opr[1]
        if opr != "ERROR" or "":
            return opr
        else:
            return "Unknown"
    except IndexError:
        blink_rgb_led('R', 1, 0.2)
        blink_rgb_led('B', 1, 0.2)
        blink_rgb_led('R', 1, 0.2)
        blink_rgb_led('B', 1, 0.2)
        print("Get operator index error.")
        return get_gsm_operator()

def get_gsm_netStrength():
    ntsr = sendCMD_waitResp("AT+CSQ\r\n", uart1)
    try:
        ntsr = ntsr[1]
        ntsr = ntsr[6:]
        ntsr = ntsr.split(",")
        ntsr = ntsr[0]
        if int(ntsr) > 0 and int(ntsr) < 10:
            net_condition = "Marginal"
        elif int(ntsr) >= 10 and int(ntsr) < 15:
            net_condition = "OK"
        elif int(ntsr) >= 15 and int(ntsr) < 20:
            net_condition = "Good"
        elif int(ntsr) >= 20 and int(ntsr) < 30:
            net_condition = "Excellent"
        else:
            net_condition = "Unknown"
        s_percentage = (int(ntsr)/30) * 100
        s_data = [str(ntsr), str(net_condition), str(s_percentage)]
        return s_data
    except Exception:
        s_data = [0, 0, 0]
        return s_data

def rgb_led_off():
    LED_RGB_R.low()
    LED_RGB_G.low()
    LED_RGB_B.low()
    return True

def send_sms(sms_content, to_phoneNumber):
    pass
