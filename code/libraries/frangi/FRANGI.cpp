/**
 * @file FRANGIcpp
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

#include "FRANGI.hpp"

#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <cerrno>
#include <cstdio>
#include <cmath>

namespace FRANGI {

	cv::Mat frangi(float scale, cv::Mat& photo){
		
		cv::Mat blur;
		cv::Mat photo_frangi;

		// APLICA UN FILTRO GAUSSIANO CON UN SIGMA = SCALE
		cv::GaussianBlur(photo, blur, cv::Size(0,0), scale, scale);


		//PASA DE UCHAR A FLOTANTE
		blur.convertTo(blur, CV_32F, 1.0/255.0);


		// CREA LA MATRIZ DERIVADA DE Lxx, Lyy, Lxy
		cv::Mat Lxx, Lyy, Lxy;


		//CALCULA LA MATRIZ GAUSSEANA
		int ksize = 2 * ceil(3 * scale) + 1;
		int half = ksize / 2;


		//GENERA LA MATRIZ CON 0
		cv::Mat kernel_xx = cv::Mat::zeros(ksize, ksize, CV_32F);
		cv::Mat kernel_yy = cv::Mat::zeros(ksize, ksize, CV_32F);
		cv::Mat kernel_xy = cv::Mat::zeros(ksize, ksize, CV_32F);

		for(int i = 0; i < ksize; i++)
		{
			for(int j = 0; j < ksize; j++)
			{
				double x = j - half;
				double y = i - half;

				double G = exp(-(x*x + y*y)/ (2*scale*scale));
				double Gxx = ((x*x - scale*scale) / (scale*scale*scale*scale)) * G;
				double Gyy = ((y*y - scale*scale) / (scale*scale*scale*scale)) * G;
				double Gxy = ((x*y)/(scale*scale*scale*scale)) * G;

				kernel_xx.at<float>(i, j) = Gxx;
				kernel_yy.at<float>(i, j) = Gyy;
				kernel_xy.at<float>(i, j) = Gxy;

			}
		}

		cv::filter2D(blur, Lxx, -1, kernel_xx);
		cv::filter2D(blur, Lyy, -1, kernel_yy);
		cv::filter2D(blur, Lxy, -1, kernel_xy);


		// EIGEN VALUES
		cv::Mat traza = Lxx + Lyy;
		
		cv::Mat Lxx_Lyy, Lxy_Lxy;
		cv::multiply(Lxx, Lyy, Lxx_Lyy);
		cv::multiply(Lxy, Lxy, Lxy_Lxy);

		cv::Mat deter = Lxx_Lyy - Lxy_Lxy;

		cv::Mat traza_half = traza / 2.0f;
		cv::Mat traza_half_sq;
		cv::multiply(traza_half, traza_half, traza_half_sq);

		cv::Mat cof_sqrt;
		cv::max(traza_half_sq - deter, 0.0f, cof_sqrt);
		cv::Mat discr;
		cv::sqrt(cof_sqrt, discr);

		cv::Mat lambda1 = traza_half - discr;
		cv::Mat lambda2 = traza_half + discr;


		//RATIOS
		cv::Mat RB;
		cv::divide(lambda1, lambda2, RB);

		cv::Mat L1_L1, L2_L2;

		cv::multiply(lambda1, lambda1, L1_L1);
		cv::multiply(lambda2, lambda2, L2_L2);
		
		cv::Mat suma_L;
		cv::add(L1_L1, L2_L2, suma_L);
		cv::Mat S;
		cv::sqrt(suma_L, S);

		// CALCULA V
		double c;
		cv::minMaxLoc(S, nullptr, &c);
		c = c / 2.0;

		cv::Mat S_S;
		cv::multiply(S, S, S_S);

		cv::Mat RB_RB;
		cv::multiply(RB, RB, RB_RB);

		cv::Mat coef1;
		cv::exp(-RB_RB / (2 * 0.5 * 0.5), coef1);

		cv::Mat coef2;
		cv::exp(-S_S / (2 * c * c), coef2);

		cv::Mat V;
		cv::multiply(coef1, (1 - coef2), V);


		cv::Mat mascara = (lambda2 > 0);
		V.setTo(0, ~mascara);

		cv::normalize(V, photo_frangi, 0, 255, cv::NORM_MINMAX);
		photo_frangi.convertTo(photo_frangi, CV_8U);
		

		return photo_frangi;
	}
}
