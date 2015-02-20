### Camera interupt pin ###
echo "23" > /sys/class/gpio/export
echo "in" > /sys/class/gpio/gpio23/direction
chmod 666 /sys/class/gpio/gpio23/value


### Status LED ###
echo "21" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio21/direction
chmod 666 /sys/class/gpio/gpio21/value

