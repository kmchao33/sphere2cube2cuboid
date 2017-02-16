#include "mHeader.h"

cv::Mat* mergeCuboidPieces(std::vector<cv::Mat*> outputs_f, std::vector<cv::Mat*> outputs_t, std::vector<cv::Mat*> outputs_r, int cuboidFaceIndex)
{
    cv::Mat *img1 = NULL, *img2 = NULL, *img3 = NULL, *img4 = NULL;
    switch(cuboidFaceIndex) {
        case 0:
            img1 = outputs_f[0];
            img2 = outputs_f[4];
            img3 = outputs_f[2];
            img4 = outputs_f[6];
            break;
        case 1:
            img1 = outputs_f[5];
            img2 = outputs_f[1];
            img3 = outputs_f[7];
            img4 = outputs_f[3];
            break;
        case 2:
            img1 = outputs_t[5];
            img2 = outputs_t[4];
            img3 = outputs_t[1];
            img4 = outputs_t[0];
            break;
        case 3:
            img1 = outputs_t[3];
            img2 = outputs_t[2];
            img3 = outputs_t[7];
            img4 = outputs_t[6];
            break;
        case 4:
            img1 = outputs_r[1];
            img2 = outputs_r[0];
            img3 = outputs_r[3];
            img4 = outputs_r[2];
            break;
        case 5:
            img1 = outputs_r[4];
            img2 = outputs_r[5];
            img3 = outputs_r[6];
            img4 = outputs_r[7];
            break;
        default:
            break;
    }
	
    int h = img1->rows + img3->rows; // height
    int w = img1->cols + img2->cols; // width

    cv::Mat *img = new cv::Mat(h, w, CV_8UC3, cv::Scalar(0, 0, 0));
	
	{
        cv::Mat part;
        part = (*img)(cv::Rect(0, 0, img1->cols, img1->rows));
        img1->copyTo(part);
        //cv::imshow("img", *img);
        //cv::waitKey(0);
	}
	{
        cv::Mat part;
        part = (*img)(cv::Rect(0, img1->rows, img3->cols, img3->rows));
        img3->copyTo(part);
        //cv::imshow("img", *img);
        //cv::waitKey(0);
	}
	{
        cv::Mat part;
        part = (*img)(cv::Rect(img1->cols, 0, img2->cols, img2->rows));
        img2->copyTo(part);
        //cv::imshow("img", *img);
        //cv::waitKey(0);
	}
	{
        cv::Mat part;
        part = (*img)(cv::Rect(img1->cols, img1->rows, img4->cols, img4->rows));
        img4->copyTo(part);
        //cv::imshow("img", *img);
        //cv::waitKey(0);
	}
	//cv::imshow("img", *img);
	//cv::waitKey(0);
	return img;
}

void test_mergeCuboidPieces()
{
	std::vector<cv::Mat*> outputs_f, outputs_t, outputs_r, cuboidImg;

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_1_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_2_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_3_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_4_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_5_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_6_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_7_f.jpg"); outputs_f.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_8_f.jpg"); outputs_f.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_1_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_2_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_3_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_4_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_5_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_6_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_7_r.jpg"); outputs_r.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_8_r.jpg"); outputs_r.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_1_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_2_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_3_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_4_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_5_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_6_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_7_t.jpg"); outputs_t.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("tmpFolder/miniCuboid_8_t.jpg"); outputs_t.push_back(img); }
	
	/*
	cv::imshow("img1", *outputs_t[5]);
	cv::imshow("img2", *outputs_t[4]);
	cv::imshow("img3", *outputs_t[1]);
	cv::imshow("img4", *outputs_t[0]);
	cv::waitKey(0);*/

	//cv::Mat *img = mergeCuboidPieces(outputs_f, outputs_t, outputs_r, 2);
	//cv::imwrite("output/cuboidImg_0.jpg",  *img);

	
	for(int i=0 ; i<6 ; i++) {
		cv::Mat *img = mergeCuboidPieces(outputs_f, outputs_t, outputs_r, i);
		cuboidImg.push_back(img);
	}
	cv::imwrite("output/cuboidImg_0.jpg",  *cuboidImg[0]);
	cv::imwrite("output/cuboidImg_1.jpg",  *cuboidImg[1]);
	cv::imwrite("output/cuboidImg_2.jpg",  *cuboidImg[2]);
	cv::imwrite("output/cuboidImg_3.jpg",  *cuboidImg[3]);
	cv::imwrite("output/cuboidImg_4.jpg",  *cuboidImg[4]);
	cv::imwrite("output/cuboidImg_5.jpg",  *cuboidImg[5]);
	
}
