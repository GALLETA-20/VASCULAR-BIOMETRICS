#!/bin/bash

g++ -I ../../vessel_biometric/libraries/camera \
	../src/example_camera.cpp \
	../../vessel_biometric/libraries/camera/CAMERA.cpp\
	-o ../bin/example_camera \
	$(pkg-config --cflags --libs opencv4)
