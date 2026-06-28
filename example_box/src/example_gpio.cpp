#include "GPIO.hpp"
#include <iostream>
#include <chrono>
#include <string>
#include <thread>

#define GPIOCHIP "/dev/gpiochip0"
#define PIN_IR 17
#define CONSUMER "LED4_IR"

int main()
{
	try
	{

		gpioSet LED_IR(GPIOCHIP, PIN_IR, CONSUMER);
		std::cout << "CONFIGURACIÓN DEL PIN " << PIN_IR << " EXITOSO!" << std::endl;

		LED_IR.ON();
		std::cout << "PIN " << PIN_IR << " EN ALTO!" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		LED_IR.OFF();
		std::cout << "PIN " << PIN_IR << " EN BAJO" << std::endl;

	} catch (const std::exception& e) {

		std::cerr << "ALGO FALLÓ: " << e.what() << std::endl;
		return -1;

	}

	return 0;
}
