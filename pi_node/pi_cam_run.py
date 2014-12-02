import RPi.GPIO as GPIO  
GPIO.setmode(GPIO.BCM)  
  
# GPIO 23 set up as input. It is pulled up to stop false signals  
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  
  
def my_callback(channel):
	print "hit the interrupt\n" 
#interrupt code here
	
	
GPIO.add_event_detect(23, GPIO.RISING, callback=my_callback, bouncetime=200) 

while True:
	pass
