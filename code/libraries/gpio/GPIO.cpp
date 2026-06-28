/**
 * @file GPIO.cpp
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

gpioSet::gpioSet(const std::string& gpioChip, int pin, const std::string& name_consumer)
{

	numPin = pin; //Guardamos el pin en variable privada global

	// CONFIGURACIÓN DE LINEA
	gpiod::line_settings line_stg;
	line_stg.set_direction(gpiod::line::direction::OUTPUT);

	// ASIGNACIÓN DE CONFIGURACIÓN A LINEA
	gpiod::line_config line_cfg;
	line_cfg.add_line_settings(numPin, line_stg);

	// ASIGNACIÓN DE NOMBRE DE CONSUMIDOR PARA EL KERNEL
	gpiod::request_config req_cfg;
	req_cfg.set_consumer(name_consumer);

	// APERTURA DEL CHIP GPIO Y PETICIÓN DE LA LINEA
	gpiod::chip chip(gpioChip);
	req_gpio = chip.prepare_request()
		.set_line_config(line_cfg)
		.set_request_config(req_cfg)
		.do_request();

}


void gpioSet::ON()
{	
	req_gpio->set_value(numPin, gpiod::line::value::ACTIVE);
}


void gpioSet::OFF()
{
	req_gpio->set_value(numPin, gpiod::line::value::INACTIVE);
}


gpioSet::~gpioSet()
{
	req_gpio->release();
}




