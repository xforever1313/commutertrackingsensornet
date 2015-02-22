This is the project for a trail node running on a raspberry pi.

For doxygen documentation, run "scons doxgyen" or visit http://ctsn.shendrick.net/doxygen/pinode/index.html .

To build the binary for the raspberry pi, run "scons release pi_build=1" (you will need arm-linux-gnueabihf-gcc on your path).

To run unit tests, run "scons run_test"

To build anything, make sure you build sethcommon and CTSNCommon first.

