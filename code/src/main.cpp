/**
 * @file main.cpp
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

#include <iostream>
#include <cstdio>
#include <cerrno>

#include <opencv4/opencv2/opencv.hpp>
#include "MASCARA.hpp"
#include "FRANGI.hpp"

#define img "/home/sallqa/img/"

int main()
{

	cv::Mat palma = cv::imread(img "img1.png", cv::IMREAD_GRAYSCALE);

	if(palma.empty())
	{

		std::cout << "ERROR AL LEER LA IMAGEN: " << strerror(errno) << std::endl;
		return -1;
	}

	std::cout << "IMAGEN LEIDA CORRECTAMENTE" << std::endl;

	cv::Mat palma_resize;
	cv::resize(palma, palma_resize, cv::Size(0, 0), 0.5, 0.5, cv::INTER_AREA);

	cv::Mat mascara = MASCARA::contour_binari(palma_resize);
	cv::Mat palma_free = MASCARA::back_mean(palma_resize, mascara);

	cv::Mat gaus_blur;
	cv::GaussianBlur(palma_free, gaus_blur, cv::Size(0, 0), 2, 2);

	cv::Mat palma_clahe;
	cv::createCLAHE(15.0, cv::Size(8, 8)) -> apply(gaus_blur, palma_clahe);

	cv::Mat k_post = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(51, 51));
	cv::Mat mascara_post;
	cv::erode(mascara, mascara_post, k_post);

	//cv::normalize(palma_vessne, palma_vessne, 0, 255, cv::NORM_MINMAX);
	//
	cv::Mat resultado = cv::Mat::zeros(palma_clahe.size(), CV_8UC1);

	for(float sigma = 4.0; sigma <= 20.0; sigma += 1)
	{
		cv::Mat v_sigma = FRANGI::frangi(sigma, palma_clahe);
		v_sigma.setTo(0, ~mascara_post);
		cv::max(resultado, v_sigma, resultado);
	}

	resultado.convertTo(resultado, CV_8U, 5.0);

	cv::imwrite("resultado.png", resultado);
	cv::imwrite("PALMA.png", palma_clahe);

	return 0;

}
