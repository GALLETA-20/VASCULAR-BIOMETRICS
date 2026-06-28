/**
 * @file example_gpio.cpp
 * @brief Sistema de Captura Vascular - Proyecto Vessel Biometric
 * * @author Vitaly Paolo Urbina Arévalo
 * @date Junio 2026
 * * Copyright (c) 2026 Vitaly Paolo Urbina Arévalo.
 * * Este programa es software libre: usted puede redistribuirlo y/o modificarlo
 * bajo los términos de la Licencia Pública General GNU publicada por la
 * Free Software Foundation, ya sea la versión 3 de la Licencia, o (a su
 * elección) cualquier versión posterior.
 * * Este programa se distribuye con la esperanza de que sea útil, pero
 * SIN NINGUNA GARANTÍA; ni siquiera la garantía implícita de
 * MERCANTILIDAD o APTITUD PARA UN PROPÓSITO PARTICULAR. Consulte la
 * Licencia Pública General GNU para más detalles.
 * * Debería haber recibido una copia de la Licencia Pública General GNU
 * junto con este programa. Si no, consulte <https://www.gnu.org/licenses/>.
 */

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
