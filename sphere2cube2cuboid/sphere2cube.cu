// This is the REAL "hello world" for CUDA!
// It takes the string "Hello ", prints it, then passes it to CUDA with an array
// of offsets. Then the offsets are added in parallel to produce the string "World!"
// By Ingemar Ragnemalm 2010

#include <vector>
#include <iostream>
#include <stdio.h>
#include <helper_functions.h>
#include <helper_math.h>
#include <opencv2/opencv.hpp>
#include <stdlib.h>


//////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////

#define PI2 1.5707963f
#define PI  3.1415927f
#define TAU 6.2831853f

#define BLOCK_DIM_X 32
#define BLOCK_DIM_Y 32

__device__
float myLerp(float a, float b, float k)
{
    //return unsigned char(a * (1.f - k) + b * k);
    return a * (1.f - k) + b * k;
}

__device__
float myClamp(float value, float min, float max)
{
    if      (value < min) return min;
    else if (value > max) return max;
    else                  return value;
}

__device__
void normalize(float *v)
{
    const float k = 1.f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= k;
    v[1] *= k;
    v[2] *= k;
}


//////////////////////////////////////////////////
// _to_img
//////////////////////////////////////////////////

__device__
void rect_to_img(int h, int w, const float* v, float& i, float& j)
{
    i = h * (       acosf (v[1])        / PI);
    j = w * (0.5f + atan2f(v[0], -v[2]) / TAU);
}

//////////////////////////////////////////////////
// _to_env
//////////////////////////////////////////////////

__device__
void cube_to_env(unsigned f, float i, float j, int h, int w, float* v)
{
    const int p[6][3][3] = {
        {{  0,  0, -1 }, {  0, -1,  0 }, {  1,  0,  0 }},
        {{  0,  0,  1 }, {  0, -1,  0 }, { -1,  0,  0 }},
        {{  1,  0,  0 }, {  0,  0,  1 }, {  0,  1,  0 }},
        {{  1,  0,  0 }, {  0,  0, -1 }, {  0, -1,  0 }},
        {{  1,  0,  0 }, {  0, -1,  0 }, {  0,  0,  1 }},
        {{ -1,  0,  0 }, {  0, -1,  0 }, {  0,  0, -1 }},
    };

    const float y = 2.0f * i / h - 1.0f;
    const float x = 2.0f * j / w - 1.0f;

    v[0] = p[f][0][0] * x + p[f][1][0] * y + p[f][2][0];
    v[1] = p[f][0][1] * x + p[f][1][1] * y + p[f][2][1];
    v[2] = p[f][0][2] * x + p[f][1][2] * y + p[f][2][2];

    normalize(v);
}

//////////////////////////////////////////////////
// Filters
//////////////////////////////////////////////////

__device__
void filter_linear(const unsigned char* srcImg, int srcHeight, int srcWidth, float i, float j, float* p)
{
    const float ii = myClamp(i - 0.5f, 0.0f, srcHeight - 1.0f);
    const float jj = myClamp(j - 0.5f, 0.0f, srcWidth - 1.0f);

    const long  i0 = lrintf(floorf(ii)), i1 = lrintf(ceilf(ii));
    const long  j0 = lrintf(floorf(jj)), j1 = lrintf(ceilf(jj));

    const float di = ii - i0;
    const float dj = jj - j0;
    /* int x = blockDim.x*blockIdx.x + threadIdx.x; */
    /* int y = blockDim.y*blockIdx.y + threadIdx.y; */
    /* printf("[%d, %d] => [%d, %d, %d, %d]\n", x, y, i0, j0, i1, j1); */
    for(int k=0; k<3; k++) {
        p[k] += myLerp(
            myLerp(srcImg[i0*srcWidth*3 + j0*3 + k], srcImg[i0*srcWidth*3 + j1*3 + k], dj),
            myLerp(srcImg[i1*srcWidth*3 + j0*3 + k], srcImg[i1*srcWidth*3 + j1*3 + k], dj),
            di
            );
    }
}

//////////////////////////////////////////////////
// supersample
//////////////////////////////////////////////////

__global__
void cudaRoutine(
    const unsigned char *srcImg,
    unsigned char *dstImgs,
    const unsigned srcHeight,
    const unsigned srcWidth,
    const unsigned dstLen
)
{
    for(int f = 0; f < 6; f++) {
        int i = blockDim.x*blockIdx.x + threadIdx.x;
        int j = blockDim.y*blockIdx.y + threadIdx.y;

        // Pattern
        const int rgss_pattern_size = 4;
        const float rgss_pattern[4][2] = {
            { 0.125f, 0.625f },
            { 0.375f, 0.125f },
            { 0.625f, 0.875f },
            { 0.875f, 0.375f }
        };

        float p[3] = { 0.0f, 0.0f, 0.0f };
        float v[3] = { 0.0f, 0.0f, 0.0f };
        float I, J;

        for(int k=0 ; k<rgss_pattern_size ; k++) {
            const float ii = rgss_pattern[k][0] + i;
            const float jj = rgss_pattern[k][1] + j;

            cube_to_env(f, ii, jj, dstLen, dstLen, v);
            rect_to_img(srcHeight, srcWidth, v, I, J);
            filter_linear(srcImg, srcHeight, srcWidth, I, J, p);
        }
        for(int k=0 ; k<3 ; k++) {
            dstImgs[(f*dstLen*dstLen*3)+(i*dstLen+j)*3 + k] = p[k] / rgss_pattern_size;
        }
    }
}

void sphere2cube(unsigned char* srcImg, 
        std::vector<unsigned char*>& dstImgs, 
        const unsigned int srcHeight, 
        const unsigned int srcWidth, 
        const unsigned int dstLen)
{
    // 3 : channels, 6 : output has 6 images
    const unsigned int srcSize = 3 * srcHeight * srcWidth;
    const unsigned int dstSize = 3 * dstLen * dstLen;
    static unsigned char *dSrcImg = NULL;
    static unsigned char *dDstImgs = NULL;

    if(!dSrcImg) {
        cudaMalloc( (void**)&dSrcImg, srcSize * sizeof(unsigned char));
        cudaMalloc( (void**)&dDstImgs, 6 * dstSize * sizeof(unsigned char));
    }

    cudaMemcpy(dSrcImg, srcImg, srcSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

    dim3 dimGrid(BLOCK_DIM_X, BLOCK_DIM_Y);
    dim3 dimBlock(dstLen/BLOCK_DIM_X, dstLen/BLOCK_DIM_Y);

    cudaRoutine<<<dimGrid, dimBlock>>>(dSrcImg, dDstImgs, srcHeight, srcWidth, dstLen);

    for(int i = 0 ; i < 6; i++) {
        cudaMemcpy(dstImgs[i], &dDstImgs[i*dstSize], dstSize, cudaMemcpyDeviceToHost);
    }
}
