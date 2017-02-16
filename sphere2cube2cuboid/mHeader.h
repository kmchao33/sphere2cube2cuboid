#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>
#include "buffer.h"

void sphere2cube(unsigned char* src, std::vector<unsigned char*>& dst, const unsigned int srcHeight, const unsigned int srcWidth, const unsigned int dstLen);

void cube2cuboid(std::vector<cv::Mat*>& cubeImg, Buffer* buf);

void cutIntoFour(std::vector<cv::Mat*> &cubeImg, std::vector<cv::Mat*> &miniImgs);

void separateCubeMini(std::vector<cv::Mat*> &miniImgs, std::vector<cv::Mat*> &frags,
	int miniCubeIndex, int* pinpoint,
	int& t, int& v, int& u, int& w
);

void buildMiniCuboid(std::vector<cv::Mat*> &frags,
	std::vector<cv::Mat*>& outputs_f, std::vector<cv::Mat*>& outputs_t, std::vector<cv::Mat*>& outputs_r,
	int miniCubeIndex, int *pinpoint, int t, int v, int u, int w
);

cv::Mat* mergeCuboidPieces(std::vector<cv::Mat*> outputs_f, std::vector<cv::Mat*> outputs_t, std::vector<cv::Mat*> outputs_r, int cuboidFaceIndex);



// test
void test_cube2cuboid();
void test_cutIntoFour();
void test_separateCubeMini();
void test_buildMiniCuboid();
void test_mergeCuboidPieces();
void test_sphere2cube(unsigned size);
void test_sphere2cube2cuboid(unsigned size);

// Utils

void rot90(cv::Mat &matImage, int k);
void rot90(cv::Mat *matImage, int k);

inline void fliplr(cv::Mat &matImage)
{
  flip(matImage, matImage, 1);
}
inline void fliplr(cv::Mat *matImage)
{
  flip(*matImage, *matImage, 1);
}

inline void flipud(cv::Mat &matImage)
{
  flip(matImage, matImage, 0);
}
inline void flipud(cv::Mat *matImage)
{
  flip(*matImage, *matImage, 0);
}

/*
inline void fliplr(cv::Mat &matImage);
inline void fliplr(cv::Mat *matImage);
inline void flipud(cv::Mat &matImage);
inline void flipud(cv::Mat *matImage);
*/

inline void test() { std::cout << "test" << std::endl; }
