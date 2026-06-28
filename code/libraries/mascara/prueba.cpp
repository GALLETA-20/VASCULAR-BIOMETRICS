#include "MASCARA.hpp"

#include "opencv4/opencv2/opencv.hpp"
#include <iostream>
#include <cerrno>
#include <cstdio>


int main()
{

	cv::Mat foto = cv::imread("captura.png", cv::IMREAD_GRAYSCALE);
	if(foto.empty())
	{
		std::cout << "ERROR AL LEER ARCHIVO: " << strerror(errno) << std::endl;
		return -1;
	}

	cv::Mat foto_resize;
	cv::resize(foto, foto_resize, cv::Size(0, 0), 0.5, 0.5, cv::INTER_AREA); 

	cv::Mat mascara =  MASCARA::contour_binari(foto_resize);

	cv::Mat foto_lista = MASCARA::back_mean(foto_resize, mascara);


	cv::imwrite("RESULTADO.png", foto_lista);
	return 0;
}
