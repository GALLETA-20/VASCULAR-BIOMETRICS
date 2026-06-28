/**
 * @file MASCARA.cpp
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

#include "MASCARA.hpp"

#include <opencv4/opencv2/opencv.hpp>
#include <cstdio>
#include <cerrno>
#include <iostream>



namespace MASCARA
{

	cv::Mat contour_binari(cv::Mat& foto)
	{

		cv::Mat img_cont;

		cv::threshold(foto, img_cont, 80, 255, cv::THRESH_BINARY);

		cv::Mat k25 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(25, 25));
		cv::Mat k7 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));

		cv::morphologyEx(img_cont, img_cont, cv::MORPH_CLOSE, k25);
		cv::morphologyEx(img_cont, img_cont, cv::MORPH_OPEN, k7);


		std::vector<std::vector<cv::Point>> contornos;
		cv::findContours(img_cont, contornos, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		double area_mayor = 0;
		int idx_mayor = 0;

		for(int i = 0; i < contornos.size(); i++)
		{

			double area = cv::contourArea(contornos[i]);
			
			if(area > area_mayor ){
			
				area_mayor= area;
				idx_mayor = i;
			}
		}

		cv::Mat max_area = cv::Mat::zeros(img_cont.size(), CV_8UC1);
		cv::drawContours(max_area, contornos, idx_mayor, 255, cv::FILLED);

		cv::Mat k_ero = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
		cv::erode(max_area, max_area, k_ero);


		return max_area;
	}
	
	cv::Mat back_mean(cv::Mat& foto, cv::Mat& mascara)
	{
		cv::Scalar media = cv::mean(foto, mascara);
		
		foto.setTo(media[0], ~mascara);


		return foto;
	}

}
