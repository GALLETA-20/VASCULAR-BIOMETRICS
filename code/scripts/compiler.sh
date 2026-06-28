#!/bin/bash

g++ \
	-I libraries/camera \
	-I libraries/gpio \
	main.cpp \
	libraries/camera/CAMERA.cpp \
	libraries/gpio/GPIO.cpp \
	-o biometric_capture \
	$(pkg-config --cflags --libs opencv4) \
	-lgpiodcxx
