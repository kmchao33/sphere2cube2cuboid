#ifndef __M_HEADER_H__
#define __M_HEADER_H__

#include "mHeader.h"

void cutIntoFour(std::vector<cv::Mat*> &cubeImg, std::vector<cv::Mat*> &miniImgs)
{
	if(cubeImg.size() != 6) return;
	if(miniImgs.size() != 0) return;

	int height = cubeImg[0]->size().height;
	int width = cubeImg[0]->size().width;

	for(int i=0 ; i<6 ; i++) {
		cv::Mat *img = cubeImg[i];
		cv::Mat *part1 = new cv::Mat();
		(*part1) = (*img)(cv::Rect(0, 0, width/2, height/2));

		cv::Mat *part2 = new cv::Mat();
		(*part2) = (*img)(cv::Rect(width/2, 0, width/2, height/2));

		cv::Mat *part3 = new cv::Mat();
		(*part3) = (*img)(cv::Rect(0, height/2, width/2, height/2));

		cv::Mat *part4 = new cv::Mat();
		(*part4) = (*img)(cv::Rect(width/2, height/2, width/2, height/2));

		miniImgs.push_back(part1);
		miniImgs.push_back(part2);
		miniImgs.push_back(part3);
		miniImgs.push_back(part4);
	}
}

void test_cutIntoFour()
{
	std::vector<cv::Mat*> cubeImg, miniImgs;
	
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_0.tiff");
		cubeImg.push_back(img);
	}
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_1.tiff");
		cubeImg.push_back(img);
		std::cout << img->size().width << std::endl;
	}
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_2.tiff");
		cubeImg.push_back(img);
	}
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_3.tiff");
		cubeImg.push_back(img);
	}
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_4.tiff");
		cubeImg.push_back(img);
	}
	{
		cv::Mat *img = new cv::Mat();
		(*img) = cv::imread("cube/cube_5.tiff");
		cubeImg.push_back(img);
	}

	//std::cout << "## Before:" << std::endl;
	//std::cout << "cubeImg.size() = " << cubeImg.size() << std::endl;
	//std::cout << "miniImgs.size() = " << miniImgs.size() << std::end;
	std::cout << "## Before:" << std::endl;
	std::cout << "cubeImg.size() = " << cubeImg.size() << std::endl;
	std::cout << "miniImgs.size() = " << miniImgs.size() << std::endl;

	cutIntoFour(cubeImg, miniImgs);

	std::cout << "## After:" << std::endl;
	std::cout << "cubeImg.size() = " << cubeImg.size() << std::endl;
	std::cout << "miniImgs.size() = " << miniImgs.size() << std::endl;

	cv::imwrite("output/miniImgs_0_1.jpg", *miniImgs[0]);
	cv::imwrite("output/miniImgs_0_2.jpg", *miniImgs[1]);
	cv::imwrite("output/miniImgs_0_3.jpg", *miniImgs[2]);
	cv::imwrite("output/miniImgs_0_4.jpg", *miniImgs[3]);

	cv::imwrite("output/miniImgs_1_1.jpg", *miniImgs[4]);
	cv::imwrite("output/miniImgs_1_2.jpg", *miniImgs[5]);
	cv::imwrite("output/miniImgs_1_3.jpg", *miniImgs[6]);
	cv::imwrite("output/miniImgs_1_4.jpg", *miniImgs[7]);

	cv::imwrite("output/miniImgs_2_1.jpg", *miniImgs[8]);
	cv::imwrite("output/miniImgs_2_2.jpg", *miniImgs[9]);
	cv::imwrite("output/miniImgs_2_3.jpg", *miniImgs[10]);
	cv::imwrite("output/miniImgs_2_4.jpg", *miniImgs[11]);

	cv::imwrite("output/miniImgs_3_1.jpg", *miniImgs[12]);
	cv::imwrite("output/miniImgs_3_2.jpg", *miniImgs[13]);
	cv::imwrite("output/miniImgs_3_3.jpg", *miniImgs[14]);
	cv::imwrite("output/miniImgs_3_4.jpg", *miniImgs[15]);

	cv::imwrite("output/miniImgs_4_1.jpg", *miniImgs[16]);
	cv::imwrite("output/miniImgs_4_2.jpg", *miniImgs[17]);
	cv::imwrite("output/miniImgs_4_3.jpg", *miniImgs[18]);
	cv::imwrite("output/miniImgs_4_4.jpg", *miniImgs[19]);

	cv::imwrite("output/miniImgs_5_1.jpg", *miniImgs[20]);
	cv::imwrite("output/miniImgs_5_2.jpg", *miniImgs[21]);
	cv::imwrite("output/miniImgs_5_3.jpg", *miniImgs[22]);
	cv::imwrite("output/miniImgs_5_4.jpg", *miniImgs[23]);

	for(unsigned i=0 ; i<miniImgs.size() ; i++) {
		delete miniImgs[i];
	}
}

#endif
