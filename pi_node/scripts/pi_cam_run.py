#!/usr/bin/python

import RPi.GPIO as GPIO
import argparse
import picamera
import os
import subprocess
import time
import signal
import sys
GPIO.setmode(GPIO.BCM)

userAgent = ""
port = "80"
uri = "/picture_parse"

def gpio_callback(channel):
    global userAgent
    global port
    global uri
    with picamera.PiCamera() as camera:
        camera.resolution = (320, 240)
        time.sleep(2)
        pic = 'pic' + time.strftime("%m_%d_%Y_%H_%M_%S") + '.jpg'
        camera.capture(pic, 'jpeg', True)

        subprocess.call(['curl', '-X', 'POST', '-A', userAgent,
                         '--data',
                         'picture=' + os.path.abspath('./' + pic) + '&remove=true',
                         'http://localhost:' + port + uri])

def signal_handler(signal, frame):
    print ("Exiting Pi Cam Script...")
    sys.exit(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--user_agent",
                        type=str,
                        dest = "user_agent",
                        help="The user agent when posting.")

    parser.add_argument("--uri",
                        type=str,
                        dest='uri',
                        help="The uri")

    parser.add_argument("--port",
                        type=str,
                        dest = "port",
                        help="The port to post to.")

    args = parser.parse_args()

    userAgent = args.user_agent
    uri = args.uri
    port = args.port

    #GPIO 23 set up as input. It is pulled up to stop false signals
    GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
    GPIO.add_event_detect(23, GPIO.RISING, callback=gpio_callback, bouncetime=200)

    signal.signal(signal.SIGINT, signal_handler)

    print ("Running Pi Cam Script...")
    signal.pause()
