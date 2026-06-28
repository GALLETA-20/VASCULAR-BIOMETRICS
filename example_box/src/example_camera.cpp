/**
 * @file example_camera.cpp
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

#include "CAMERA.hpp"
#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <cerrno>
#include <cstring>


int main()
{
	cv::Mat foto = cap_cam::cap_cam_ir();
	if(foto.empty())
	{
		std::cout << "ALGO FALLÓ [1]: " << strerror(errno) << std::endl;
	}

	std::cout << "IMAGEN LEÍDA EXITOSAMENTE!\n";

	cv::imwrite("foto.png", foto);
	std::cout << "IMAGEN GUARDAD: REVISA foto.png\n" << std::endl;

	return 0;
}
