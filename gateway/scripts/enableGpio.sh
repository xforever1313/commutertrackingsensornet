echo "23" > sys/class/gpio/export
echo "out" > sys/class/gpio/gpio23/direction 
chmod 666 sys/class/gpio/gpio23/value

