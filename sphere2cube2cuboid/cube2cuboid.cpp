#include "mHeader.h"
#include <omp.h>
using namespace std;
void cube2cuboid(std::vector<cv::Mat*>& cubeImg, Buffer* buf)
{

    int pinpoints3[8][3] = {
        {1, 45, 365}, {2, 980, 365},
        {1, 45, 625}, {2, 980, 625},
        {1, 870, 365}, {2, 155, 365},
        {1, 870, 625}, {2, 155, 625}
    };

    std::vector<cv::Mat*> miniImgs;
    cutIntoFour(cubeImg, miniImgs);

	std::vector<cv::Mat*> outputs_f, outputs_t, outputs_r;
    omp_set_dynamic(0);     // Explicitly disable dynamic teams
    omp_set_num_threads(8); // Use 8 threads for all consecutive parallel regions
    #pragma omp parallel
    {
        std::vector<cv::Mat*> frags, f_private, t_private, r_private;
        int t, v, u, w;
        #pragma omp for
        for(int i=0 ; i<8 ; i++) {
            separateCubeMini(miniImgs, frags, i, pinpoints3[i], t, v, u, w);
            buildMiniCuboid(frags, f_private, t_private, r_private, i, pinpoints3[i], t, v, u, w); // TODO:
        }
        #pragma omp for schedule(static) ordered
        for(int i = 0; i < omp_get_num_threads(); i++) {
            #pragma omp ordered
            outputs_f.insert(outputs_f.end(), f_private.begin(), f_private.end());
            outputs_t.insert(outputs_t.end(), t_private.begin(), t_private.end());
            outputs_r.insert(outputs_r.end(), r_private.begin(), r_private.end());
        }
        for(unsigned i=0 ; i<frags.size() ; i++) {
            delete frags[i];
        }
        #pragma omp for schedule(static) ordered
        for(int i=0 ; i<6 ; i++) {
            cv::Mat* result = mergeCuboidPieces(outputs_f, outputs_t, outputs_r, i);
            buf->widths[i] = result->size().width;
            buf->heights[i] = result->size().height;
            memcpy(buf->pointers[i], result->data, result->total() * result->elemSize());
            delete result;
        }
    }
    
    // free the space
    for(unsigned i=0 ; i<miniImgs.size() ; i++) {
        delete miniImgs[i];
    }
	for(unsigned i=0 ; i<outputs_f.size() ; i++) {
		delete outputs_f[i];
		delete outputs_t[i];
		delete outputs_r[i];
	}
}
