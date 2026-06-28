#!/bin/bash

g++ -I ../../vessel_biometric/libraries/gpio \
	../src/example_gpio.cpp \
	../../vessel_biometric/libraries/gpio/GPIO.cpp \
	-o ../bin/example_gpio -lgpiodcxx
