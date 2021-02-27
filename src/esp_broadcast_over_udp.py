# Below code is used to broadcast data over UDP on a WiFi network using ESP8266 with Micropython firmware
# This file must be loaded in ESP8266 only!

import gc
import time
from socket import *
from machine import UART
import uos


def do_connect():
    import network
    sta_if = network.WLAN(network.STA_IF)
    # ap_if is access point config. You can use it to turn off access point on your chip
    # ap_if = network.WLAN(network.AP_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect('PiFi', 'sgmp1357')
        while not sta_if.isconnected():
            pass
        # ap_if.active(False)
    print('network config:', sta_if.ifconfig())


def broadcast(message: bytes):
    cs = socket(AF_INET, SOCK_DGRAM)
    cs.sendto(message, ('192.168.0.6', 7007))


do_connect()
time.sleep(10)
uos.dupterm(None, 1)    # Detach UART from REPL
uart = UART(0, 115200)  # init with given baudrate
uart.write(bytearray('ready'))
line = bytes()

while True:
    if uart.any():
        line = uart.readline()
        broadcast(line)
        line = ""
        uart.write(bytearray("ok"))
        time.sleep(2)
        gc.collect()
