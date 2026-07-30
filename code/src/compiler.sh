#!/bin/bash
set -e
g++ -I ../libraries/camera/ -I ../libraries/gpio/ -I ../libraries/mascara/ -I ../libraries/pre_poc/ \
    enrollment1.cpp ../libraries/camera/CAMERA.cpp ../libraries/gpio/GPIO.cpp \
    ../libraries/mascara/MASCARA.cpp \
    -o enrollment1 \
    `pkg-config --cflags --libs opencv4` -lgpiodcxx -lgpiod -lsqlite3
echo "enrollment1 compilado OK"

g++ -I ../libraries/camera/ -I ../libraries/gpio/ -I ../libraries/mascara/ -I ../libraries/pre_poc/ \
    verificar1.cpp ../libraries/camera/CAMERA.cpp ../libraries/gpio/GPIO.cpp \
    ../libraries/mascara/MASCARA.cpp \
    -o verificar1 \
    `pkg-config --cflags --libs opencv4` -lgpiodcxx -lgpiod -lsqlite3
echo "verificar1 compilado OK"
