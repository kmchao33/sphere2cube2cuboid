#include "mHeader.h"

void buildMiniCuboid(std::vector<cv::Mat*> &frags,
	std::vector<cv::Mat*> &outputs_f, std::vector<cv::Mat*> &outputs_t, std::vector<cv::Mat*> &outputs_r,
	int miniCubeIndex, int *pinpoint, int t, int v, int u, int w
)
{
	cv::Mat *mm  = frags[0];
	cv::Mat *rm  = frags[1];
	cv::Mat *ttm = frags[2];
	cv::Mat *btm = frags[3];
	cv::Mat *tr  = frags[4];
	cv::Mat *br  = frags[5];
	cv::Mat *top = frags[6];

	cv::Mat *output_f = NULL;
	cv::Mat *output_r = NULL;
	cv::Mat *output_t = NULL;

	// output_f
	{
		output_f = new cv::Mat();
		cv::resize(*mm, *output_f, cv::Size(u, w)); //

		//cv::imshow("output_f", *output_f);
		//cv::waitKey(0);
	}
	
	// output_r
	{
		output_r = new cv::Mat(w, v, CV_8UC3, cv::Scalar(0, 0, 0)); //

		cv::Mat br_resize;
		cv::resize(*br, br_resize, cv::Size(u, w)); //

		cv::Mat rm_warp;
		{
			cv::Point2f movingPoints[] = {
				cv::Point2f(0, pinpoint[2]-cvRound(t-double(t)*w/u)),
				cv::Point2f(rm->cols-1, 0),
				cv::Point2f(0, rm->rows-1),
				cv::Point2f(rm->cols-1, rm->rows-1)
			};
			cv::Point2f fixedPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(v-u-1, 0),
				cv::Point2f(0, w-1),
				cv::Point2f(v-u-1, w-1)
			};
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(movingPoints, fixedPoints);
			cv::warpPerspective(*rm, rm_warp, perspective_matrix, cv::Size(v-u, w), cv::INTER_LINEAR); //
		}

		//cv::Mat part1 = (*output_r)(cv::Rect(0, 0, v-u, output_r->size().height)); // rm_warp->size() // TODO: ??
		cv::Mat part1 = (*output_r)(cv::Rect(0, 0, rm_warp.cols, rm_warp.rows));
		rm_warp.copyTo(part1);
		//cv::Mat part2 = (*output_r)(cv::Rect(v-u, 0, u, output_r->size().height)); // br_resize->size() // TODO: ??
		cv::Mat part2 = (*output_r)(cv::Rect(v-u, 0, br_resize.cols, br_resize.rows));
		br_resize.copyTo(part2);

		//cv::imshow("output_r", *output_r);
		//cv::waitKey(0);
	}

	// output_t
	{
		output_t = new cv::Mat(v, u, CV_8UC3, cv::Scalar(0, 0, 0)); //

		cv::Mat t_resize;
		cv::resize(*top, t_resize, cv::Size(w, w));

		cv::Mat tr_warp_rot;
		{
			cv::Point2f movingPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(tr->cols-1, 0),
				cv::Point2f(0, tr->rows-1),
				cv::Point2f(tr->cols-1, tr->rows-1)
			};
			cv::Point2f fixedPoints[] = {
				cv::Point2f(u-w, 0),
				cv::Point2f(u-1, 0),
				cv::Point2f(0, u-w-1),
				cv::Point2f(u-1, u-w-1)
			};
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(movingPoints, fixedPoints);
			cv::warpPerspective(*tr, tr_warp_rot, perspective_matrix, cv::Size(u, u-w), cv::INTER_LINEAR); //
			rot90(tr_warp_rot, 1);
		}

		cv::Mat ttm_warp;
		{
			cv::Point2f movingPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(ttm->cols-1, 0),
				cv::Point2f(0, ttm->rows-1),
				cv::Point2f(ttm->cols-1, ttm->rows-1)
			};
			cv::Point2f fixedPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(w-1, 0),
				cv::Point2f(0, u-w-1),
				cv::Point2f(u-1, u-w-1)
			};
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(movingPoints, fixedPoints);
			cv::warpPerspective(*ttm, ttm_warp, perspective_matrix, cv::Size(u, u-w), cv::INTER_LINEAR); //
		}

		cv::Mat btm_warp;
		{
			cv::Point2f movingPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(btm->cols-1, 0),
				cv::Point2f(0, btm->rows-1),
				cv::Point2f(pinpoint[1]-1, btm->rows-1)
			};
			cv::Point2f fixedPoints[] = {
				cv::Point2f(0, 0),
				cv::Point2f(u-1, 0),
				cv::Point2f(0, v-u-1),
				cv::Point2f(u-1, v-u-1)
			};
			cv::Mat perspective_matrix = cv::getPerspectiveTransform(movingPoints, fixedPoints);
			cv::warpPerspective(*btm, btm_warp, perspective_matrix, cv::Size(u, v-u), cv::INTER_LINEAR); //
		}

		cv::Mat part1 = (*output_t)(cv::Rect(0, 0, t_resize.cols, t_resize.rows)); //
		//cv::Mat part1 = (*output_t)(cv::Rect(0, 0, t_resize.size().width, t_resize.size().height)); //
		//cv::Mat part1 = (*output_t)(cv::Rect(0, 0, w, w)); //
		t_resize.copyTo(part1);
		cv::Mat part2 = (*output_t)(cv::Rect(0, w, ttm_warp.cols, ttm_warp.rows));
		//cv::Mat part2 = (*output_t)(cv::Rect(0, w, ttm_warp.size().width, ttm_warp.size().height)); //
		ttm_warp.copyTo(part2);
		//cv::Mat part3 = (*output_t)(cv::Rect(0, u, btm_warp.size().width, v-u)); //
		//cv::Mat part3 = (*output_t)(cv::Rect(0, u, btm_warp.size().width, btm_warp.size().height)); //
		cv::Mat part3 = (*output_t)(cv::Rect(0, u, btm_warp.cols, btm_warp.rows));
		btm_warp.copyTo(part3);
		cv::Mat part4 = (*output_t)(cv::Rect(w, 0, tr_warp_rot.cols, tr_warp_rot.rows)); //
		cv::Mat tmp;
		addWeighted( part4, 1.0, tr_warp_rot, 1.0, 0.0, tmp);
		tmp.copyTo(part4);
		// output_t[0:u, w:u, :] = output_t[0:u, w:u, :]+tr_warp_rot[:,:,:]
		
		//cv::imshow("part1", part1);
		//cv::imshow("part2", part2);
		//cv::imshow("part3", part3);
		//cv::imshow("part4", part4);
		//cv::imshow("output_t", *output_t);
		//cv::waitKey(0);
		
	}

	switch(miniCubeIndex) {
		case 0:
			fliplr(output_f);
			fliplr(output_r);
			fliplr(output_t);
			rot90(output_t, 1);
			break;
		case 1:
			rot90(output_t, 3);
			break;
		case 2:
			rot90(output_f, 2);
			rot90(output_r, 2);
			rot90(output_t, 1);
			break;
		case 3:
			flipud(output_f);
			flipud(output_r);
			flipud(output_t);
			rot90(output_t, 1);
			break;
		case 4:
			rot90(output_t, 1);
			break;
		case 5:
			fliplr(output_f);
			fliplr(output_r);
			fliplr(output_t);
			rot90(output_t, 3);
			break;
		case 6:
			flipud(output_f);
			flipud(output_r);
			flipud(output_t);
			rot90(output_t, 3);
			break;
		case 7:
			rot90(output_f, 2);
			rot90(output_r, 2);
			rot90(output_t, 3);
			break;
		default:
			break;
	}

	outputs_f.push_back(output_f);
	outputs_r.push_back(output_r);
	outputs_t.push_back(output_t);
	
	//cv::imshow("output_f", *output_f);
	//cv::imshow("output_r", *output_r);
	//cv::imshow("output_t", *output_t);
	//cv::waitKey(0);
}
/*
br_resize
rm_warp

TODO: check cv::Mat* declaration, whether all have new operation


		cv::Point2f movingPoints[] = {
			cv::Point2f(),
			cv::Point2f(),
			cv::Point2f(),
			cv::Point2f()
		};
		cv::Point2f fixedPoints[] = {
			cv::Point2f(),
			cv::Point2f(),
			cv::Point2f(),
			cv::Point2f()
		};
*/
void test_buildMiniCuboid()
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

	std::vector<cv::Mat*> outputs_f, outputs_r, outputs_t;

	/*
	int t, v, u, w;
	separateCubeMini(miniImgs, frags, 0, pinpoints3[0], t, v, u, w);
	buildMiniCuboid(frags, outputs_f, outputs_t, outputs_r, 0, pinpoints3[0], t, v, u, w);
	*/
	
	
	for(int i=0 ; i<8 ; i++) {
		int t, v, u, w;
		separateCubeMini(miniImgs, frags, i, pinpoints3[i], t, v, u, w);
		buildMiniCuboid(frags, outputs_f, outputs_t, outputs_r, i, pinpoints3[i], t, v, u, w); // TODO:
    }
	
	cv::imwrite("output/outputs_0_f.jpg",  *(outputs_f[0]));
	cv::imwrite("output/outputs_1_f.jpg",  *(outputs_f[1]));
	cv::imwrite("output/outputs_2_f.jpg",  *(outputs_f[2]));
	cv::imwrite("output/outputs_3_f.jpg",  *(outputs_f[3]));
	cv::imwrite("output/outputs_4_f.jpg",  *(outputs_f[4]));
	cv::imwrite("output/outputs_5_f.jpg",  *(outputs_f[5]));
	cv::imwrite("output/outputs_6_f.jpg",  *(outputs_f[6]));
	cv::imwrite("output/outputs_7_f.jpg",  *(outputs_f[7]));

	cv::imwrite("output/outputs_0_r.jpg",  *(outputs_r[0]));
	cv::imwrite("output/outputs_1_r.jpg",  *(outputs_r[1]));
	cv::imwrite("output/outputs_2_r.jpg",  *(outputs_r[2]));
	cv::imwrite("output/outputs_3_r.jpg",  *(outputs_r[3]));
	cv::imwrite("output/outputs_4_r.jpg",  *(outputs_r[4]));
	cv::imwrite("output/outputs_5_r.jpg",  *(outputs_r[5]));
	cv::imwrite("output/outputs_6_r.jpg",  *(outputs_r[6]));
	cv::imwrite("output/outputs_7_r.jpg",  *(outputs_r[7]));

	cv::imwrite("output/outputs_0_t.jpg",  *(outputs_t[0]));
	cv::imwrite("output/outputs_1_t.jpg",  *(outputs_t[1]));
	cv::imwrite("output/outputs_2_t.jpg",  *(outputs_t[2]));
	cv::imwrite("output/outputs_3_t.jpg",  *(outputs_t[3]));
	cv::imwrite("output/outputs_4_t.jpg",  *(outputs_t[4]));
	cv::imwrite("output/outputs_5_t.jpg",  *(outputs_t[5]));
	cv::imwrite("output/outputs_6_t.jpg",  *(outputs_t[6]));
	cv::imwrite("output/outputs_7_t.jpg",  *(outputs_t[7]));
	
}
