import RPi.GPIO as GPIO
import picamera
import os
import subprocess
import time
import sys
GPIO.setmode(GPIO.BCM)

sys.path.append('../')
from Secrets import *

# GPIO 23 set up as input. It is pulled up to stop false signals
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

def my_callback(channel):
    with picamera.PiCamera() as camera:
        camera.resolution = (320, 240)
        time.sleep(2)
        pic = 'pic' + time.strftime("%m_%d_%Y_%H_%M_%S") + '.jpg'
        camera.capture(pic, 'jpeg', True)

        subprocess.call(['curl', '-X', 'POST', '-A', PI_NODE_USER_AGENT,
                         '--data', 'picture_file=' + os.path.abspath('./' + pic),
                         'http://localhost:9011/picture_send'])

GPIO.add_event_detect(23, GPIO.RISING, callback=my_callback, bouncetime=200) 

while True:
    pass
