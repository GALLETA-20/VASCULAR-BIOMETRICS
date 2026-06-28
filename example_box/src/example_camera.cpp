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
