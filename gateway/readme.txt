This folder is to be used for the Gateway Node.

That is, the Raspberry Pi that will send / receive commands to / from the trail.

For doxygen documentation, run "scons doxgyen" or visit http://ctsn.shendrick.net/doxygen/gateway/index.html .

To build the binary for the raspberry pi, run "scons release pi_build=1" (you will need arm-linux-gnueabihf-gcc on your path).  For x86_64, just run "scons release".

To run unit tests, run "scons run_test"

To build anything, make sure you build sethcommon and CTSNCommon first.

