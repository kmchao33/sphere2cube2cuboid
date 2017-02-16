#include "mHeader.h"

void separateCubeMini(std::vector<cv::Mat*> &miniImgs, std::vector<cv::Mat*> &frags,
	int miniCubeIndex, int* pinpoint,
	int& t, int& v, int& u, int& w
)
{
	if(miniImgs.size() != 24) return;

	int size = miniImgs[0]->rows;
	cv::Mat pinnedImage, rightImage, topImage;
	//cv::Mat pinnedImage(512, 512, CV_8UC3, cv::Scalar(0, 0, 255));
	//cv::Mat rightImage(512, 512, CV_8UC3, cv::Scalar(0, 0, 255));
	//cv::Mat topImage(512, 512, CV_8UC3, cv::Scalar(0, 0, 255));

	cv::Mat *mm = NULL, *rm = NULL, *ttm = NULL, *btm = NULL, *tr = NULL, *br = NULL, *top = NULL;
	
	if(pinpoint[0] == 1 || pinpoint[0] == 2) {
		switch(miniCubeIndex) {
			case 0:
				miniImgs[4*0+0]->copyTo(pinnedImage); // pinnedImage
				fliplr(pinnedImage);
				miniImgs[4*4+1]->copyTo(rightImage); // rightImage
				fliplr(rightImage);
				miniImgs[4*2+3]->copyTo(topImage); // topImage
				fliplr(topImage);
				rot90(topImage, 1);
				pinpoint[1] = size - pinpoint[1] + 1;
                break;
            case 1:
				miniImgs[4*1+1]->copyTo(pinnedImage); // pinnedImage
				miniImgs[4*4+0]->copyTo(rightImage); // rightImage
				miniImgs[4*2+2]->copyTo(topImage); // topImage
				rot90(topImage, 1);
				pinpoint[1] = pinpoint[1] - size;
                break;
            case 2:
				miniImgs[4*0+2]->copyTo(pinnedImage); // pinnedImage
				rot90(pinnedImage, 2);
				miniImgs[4*4+3]->copyTo(rightImage); // rightImage
				rot90(rightImage, 2);
				miniImgs[4*3+1]->copyTo(topImage); // topImage
				rot90(topImage, 3);
				pinpoint[1] = size - pinpoint[1] + 1;
				pinpoint[2]=size - (pinpoint[2] - size) + 1;
                break;
            case 3:
				miniImgs[4*1+3]->copyTo(pinnedImage); // pinnedImage
				flipud(pinnedImage);
				miniImgs[4*4+2]->copyTo(rightImage); // rightImage
				flipud(rightImage);
				miniImgs[4*3+0]->copyTo(topImage); // topImage
				flipud(topImage);
				rot90(topImage, 1);
				pinpoint[1] = pinpoint[1] - size;
				pinpoint[2] = size - (pinpoint[2] - size) + 1;
                break;
            case 4:
				miniImgs[4*0+1]->copyTo(pinnedImage); // pinnedImage
				miniImgs[4*5+0]->copyTo(rightImage); // rightImage
				miniImgs[4*2+1]->copyTo(topImage); // topImage
				rot90(topImage, 3);
				pinpoint[1] = pinpoint[1] - size;
                break;
            case 5:
				miniImgs[4*1+0]->copyTo(pinnedImage); // pinnedImage
				fliplr(pinnedImage);
				miniImgs[4*5+1]->copyTo(rightImage); // rightImage
				fliplr(rightImage);
				miniImgs[4*2+0]->copyTo(topImage); // topImage
				fliplr(topImage);
				rot90(topImage, 3);
				pinpoint[1] = size - pinpoint[1] + 1;
                break;
            case 6:
				miniImgs[4*0+3]->copyTo(pinnedImage); // pinnedImage
				flipud(pinnedImage);
				miniImgs[4*5+2]->copyTo(rightImage); // rightImage
				flipud(rightImage);
				miniImgs[4*3+3]->copyTo(topImage); // topImage
				flipud(topImage);
				rot90(topImage, 3);
				pinpoint[1] = pinpoint[1] - size;
				pinpoint[2] = size - (pinpoint[2] - size) + 1;
                break;
            case 7:
				miniImgs[4*1+2]->copyTo(pinnedImage); // pinnedImage
				rot90(pinnedImage, 2);
				miniImgs[4*5+3]->copyTo(rightImage); // rightImage
				rot90(rightImage, 2);
				miniImgs[4*3+2]->copyTo(topImage); // topImage
				rot90(topImage, 1);
				pinpoint[1] = size - pinpoint[1] + 1;
				pinpoint[2] = size - (pinpoint[2] - size) + 1;
                break;
            default:
                break;
		}
	}

	t = pinnedImage.size().height;
	v = 512;
	u = cvRound(double(pinpoint[1]) * v / t);
	w = cvRound(double(512-pinpoint[2]) * v / t);


	cv::Mat part;

	// middle of middle
	int mm_h = t * w / v;
	int mm_w = t * u / v;
	//mm = new cv::Mat(t * w / v, t * u / v, CV_8UC3, cv::Scalar(0, 0, 255));
	mm = new cv::Mat();
	part = pinnedImage(cv::Rect(0, pinpoint[2], mm_w, mm_h));
	//part = pinnedImage(cv::Rect(0, pinpoint[2], pinpoint[1], 512));
	part.copyTo(*mm);

	// right of the middle
	int rm_h = cvRound(double(t)*w/u);
	int rm_w = t-t*u/v;
	//rm = new cv::Mat(cvRound(double(t)*w/u), t-t*u/v, CV_8UC3, cv::Scalar(0, 0, 255)); // TODO: need to use round ?
	rm = new cv::Mat();
	part = pinnedImage(cv::Rect(pinpoint[1], cvRound(t-double(t)*w/u), rm_w, rm_h));
	//part = pinnedImage(cv::Rect(pinpoint[1], cvRound(t-double(t)*w/u), 512, 512));
	part.copyTo(*rm);

	// top of top of middle
	int ttm_h = cvRound(t-double(t)*w/u);
	int ttm_w = t;
	//ttm = new cv::Mat(cvRound(t-double(t)*w/u), t, CV_8UC3, cv::Scalar(0, 0, 255));
	ttm = new cv::Mat();
	part = pinnedImage(cv::Rect(0, 0, ttm_w, ttm_h));
	//part = pinnedImage(cv::Rect(0, 0, t, cvRound(t-double(t)*w/u)));
	part.copyTo(*ttm);

	// bottom of top of middle
	int btm_h = cvRound(double(t)*w/u-double(t)*w/v);
	int btm_w = t;
	//btm = new cv::Mat(cvRound(double(t)*w/u-double(t)*w/v), t, CV_8UC3, cv::Scalar(0, 0, 255));
	btm = new cv::Mat();
	part = pinnedImage(cv::Rect(0, cvRound(t-double(t)*w/u), btm_w, btm_h));
	part.copyTo(*btm);

	// top of right
	int tr_h = cvRound(t-double(t)*w/u);
	int tr_w = t;
	//tr = new cv::Mat(cvRound(t-double(t)*w/u), t, CV_8UC3, cv::Scalar(0, 0, 255));
	tr = new cv::Mat();
	part = rightImage(cv::Rect(0, 0, tr_w, tr_h));
	part.copyTo(*tr);

	// bottom of right
	int br_h = cvRound(double(t)*w/u);
	int br_w = t;
	//br = new cv::Mat(cvRound(double(t)*w/u), t, CV_8UC3, cv::Scalar(0, 0, 255));
	br = new cv::Mat();
	part = rightImage(cv::Rect(0, cvRound(t-double(t)*w/u), br_w, br_h));
	part.copyTo(*br);

	// top
	//top = new cv::Mat(h, w, CV_8UC3, cv::Scalar(0, 0, 255));
	top = new cv::Mat();
	topImage.copyTo(*top);	

	frags.push_back(mm);
	frags.push_back(rm);
	frags.push_back(ttm);
	frags.push_back(btm);
	frags.push_back(tr);
	frags.push_back(br);
	frags.push_back(top);
}

void test_separateCubeMini()
{
	std::vector<cv::Mat*> miniImgs, frags;
	int pinpoints3[8][3] = {
        {1, 45, 365}, {2, 980, 365},
        {1, 45, 625}, {2, 980, 625},
        {1, 870, 365}, {2, 155, 365},
        {1, 870, 625}, {2, 155, 625}
    };

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_0_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_0_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_0_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_0_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_1_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_1_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_1_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_1_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_2_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_2_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_2_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_2_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_3_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_3_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_3_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_3_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_4_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_4_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_4_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_4_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_5_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_5_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_5_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("output/miniImgs_5_4.jpg"); miniImgs.push_back(img); }

	/* Visual Studio Debugging
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_0_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_0_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_0_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_0_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_1_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_1_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_1_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_1_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_2_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_2_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_2_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_2_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_3_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_3_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_3_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_3_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_4_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_4_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_4_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_4_4.jpg"); miniImgs.push_back(img); }

	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_5_1.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_5_2.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_5_3.jpg"); miniImgs.push_back(img); }
	{ cv::Mat *img = new cv::Mat(); (*img) = cv::imread("../Release/output/miniImgs_5_4.jpg"); miniImgs.push_back(img); }
	*/

	/*
	int t, v, u, w;
	separateCubeMini(miniImgs, frags, 0, pinpoints3[0], t, v, u, w);
	cv::imwrite("output/frags_0_mm.jpg",  *(frags[0 + 0]));
	cv::imwrite("output/frags_0_rm.jpg",  *(frags[0 + 1]));
	cv::imwrite("output/frags_0_ttm.jpg", *(frags[0 + 2]));
	cv::imwrite("output/frags_0_btm.jpg", *(frags[0 + 3]));
	cv::imwrite("output/frags_0_tr.jpg",  *(frags[0 + 4]));
	cv::imwrite("output/frags_0_br.jpg",  *(frags[0 + 5]));
	cv::imwrite("output/frags_0_t.jpg",   *(frags[0 + 6]));
	*/
	for(int i=0 ; i<8 ; i++) {
		int t, v, u, w;
		separateCubeMini(miniImgs, frags, i, pinpoints3[i], t, v, u, w);
    }

	cv::imwrite("output/frags_0_mm.jpg",  *(frags[0 + 0]));
	cv::imwrite("output/frags_0_rm.jpg",  *(frags[0 + 1]));
	cv::imwrite("output/frags_0_ttm.jpg", *(frags[0 + 2]));
	cv::imwrite("output/frags_0_btm.jpg", *(frags[0 + 3]));
	cv::imwrite("output/frags_0_tr.jpg",  *(frags[0 + 4]));
	cv::imwrite("output/frags_0_br.jpg",  *(frags[0 + 5]));
	cv::imwrite("output/frags_0_t.jpg",   *(frags[0 + 6]));

	cv::imwrite("output/frags_1_mm.jpg",  *(frags[7 + 0]));
	cv::imwrite("output/frags_1_rm.jpg",  *(frags[7 + 1]));
	cv::imwrite("output/frags_1_ttm.jpg", *(frags[7 + 2]));
	cv::imwrite("output/frags_1_btm.jpg", *(frags[7 + 3]));
	cv::imwrite("output/frags_1_tr.jpg",  *(frags[7 + 4]));
	cv::imwrite("output/frags_1_br.jpg",  *(frags[7 + 5]));
	cv::imwrite("output/frags_1_t.jpg",   *(frags[7 + 6]));

	cv::imwrite("output/frags_2_mm.jpg",  *(frags[14 + 0]));
	cv::imwrite("output/frags_2_rm.jpg",  *(frags[14 + 1]));
	cv::imwrite("output/frags_2_ttm.jpg", *(frags[14 + 2]));
	cv::imwrite("output/frags_2_btm.jpg", *(frags[14 + 3]));
	cv::imwrite("output/frags_2_tr.jpg",  *(frags[14 + 4]));
	cv::imwrite("output/frags_2_br.jpg",  *(frags[14 + 5]));
	cv::imwrite("output/frags_2_t.jpg",   *(frags[14 + 6]));

	cv::imwrite("output/frags_3_mm.jpg",  *(frags[21 + 0]));
	cv::imwrite("output/frags_3_rm.jpg",  *(frags[21 + 1]));
	cv::imwrite("output/frags_3_ttm.jpg", *(frags[21 + 2]));
	cv::imwrite("output/frags_3_btm.jpg", *(frags[21 + 3]));
	cv::imwrite("output/frags_3_tr.jpg",  *(frags[21 + 4]));
	cv::imwrite("output/frags_3_br.jpg",  *(frags[21 + 5]));
	cv::imwrite("output/frags_3_t.jpg",   *(frags[21 + 6]));

	cv::imwrite("output/frags_4_mm.jpg",  *(frags[28 + 0]));
	cv::imwrite("output/frags_4_rm.jpg",  *(frags[28 + 1]));
	cv::imwrite("output/frags_4_ttm.jpg", *(frags[28 + 2]));
	cv::imwrite("output/frags_4_btm.jpg", *(frags[28 + 3]));
	cv::imwrite("output/frags_4_tr.jpg",  *(frags[28 + 4]));
	cv::imwrite("output/frags_4_br.jpg",  *(frags[28 + 5]));
	cv::imwrite("output/frags_4_t.jpg",   *(frags[28 + 6]));

	cv::imwrite("output/frags_5_mm.jpg",  *(frags[35 + 0]));
	cv::imwrite("output/frags_5_rm.jpg",  *(frags[35 + 1]));
	cv::imwrite("output/frags_5_ttm.jpg", *(frags[35 + 2]));
	cv::imwrite("output/frags_5_btm.jpg", *(frags[35 + 3]));
	cv::imwrite("output/frags_5_tr.jpg",  *(frags[35 + 4]));
	cv::imwrite("output/frags_5_br.jpg",  *(frags[35 + 5]));
	cv::imwrite("output/frags_5_t.jpg",   *(frags[35 + 6]));

	cv::imwrite("output/frags_6_mm.jpg",  *(frags[42 + 0]));
	cv::imwrite("output/frags_6_rm.jpg",  *(frags[42 + 1]));
	cv::imwrite("output/frags_6_ttm.jpg", *(frags[42 + 2]));
	cv::imwrite("output/frags_6_btm.jpg", *(frags[42 + 3]));
	cv::imwrite("output/frags_6_tr.jpg",  *(frags[42 + 4]));
	cv::imwrite("output/frags_6_br.jpg",  *(frags[42 + 5]));
	cv::imwrite("output/frags_6_t.jpg",   *(frags[42 + 6]));

	cv::imwrite("output/frags_7_mm.jpg",  *(frags[49 + 0]));
	cv::imwrite("output/frags_7_rm.jpg",  *(frags[49 + 1]));
	cv::imwrite("output/frags_7_ttm.jpg", *(frags[49 + 2]));
	cv::imwrite("output/frags_7_btm.jpg", *(frags[49 + 3]));
	cv::imwrite("output/frags_7_tr.jpg",  *(frags[49 + 4]));
	cv::imwrite("output/frags_7_br.jpg",  *(frags[49 + 5]));
	cv::imwrite("output/frags_7_t.jpg",   *(frags[49 + 6]));
	
}
