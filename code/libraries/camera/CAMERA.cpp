/**
 * @file CAMERA.cpp
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
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>


namespace cap_cam
{

	namespace
	{

		std::vector<uchar> leer_datos(FILE* datos)
		{
			std::vector<uchar> buf;
			uchar tmp[4096];
			size_t n;

			while((n = fread(tmp, 1, sizeof(tmp), datos)) > 0)
			{
				buf.insert(buf.end(), tmp, tmp + n);
			}

			return buf;
		}
	}

	cv::Mat cap_cam_ir()
	{

		const char* cmd = "rpicam-still -n -t 1000 --shutter 70000 --gain 1.5 --saturation 0 --encoding png -o -";

		FILE* datos = popen(cmd, "r");

		if(!datos)
		{
			std::cerr << "ERROR AL LANZAR COMANDO [rpicam-still]: " << strerror(errno) << '\n';
			return cv::Mat();
		}

		std::vector<uchar> img_bytes = leer_datos(datos);

		int ret = pclose(datos);

		if(ret != 0 || img_bytes.empty())
		{
			std::cerr << "ALGO FALLÓ AL LEER CAPTURA DE DATOS!\n";
			return cv::Mat();
		}


		cv::Mat foto = cv::imdecode(img_bytes, cv::IMREAD_GRAYSCALE);

		if(foto.empty())
		{
			std::cerr << "ERROR AL DECODIFICAR IMAGEN, ALGO FALLÓ!\n";
			return cv::Mat();
		}

		return foto;
	}
}



