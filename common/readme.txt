This project is common things for all CTSN projects.

For doxygen documentation, run "scons doxgyen" or visit http://ctsn.shendrick.net/doxygen/common/index.html .

To build the static libs for the raspberry pi, run "scons create_lib pi_build=1" (you will need arm-linux-gnueabihf-gcc on your path).  Just run "scons create_lib" for an x86_64 build.

To run unit tests, run "scons run_test"

