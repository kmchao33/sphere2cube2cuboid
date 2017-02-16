#include "mHeader.h"

void rot90(cv::Mat &matImage, int k){
  // rot 90*k degree counterclockwise
  switch(k) {
    case 0:
      break;
    case 1:
      transpose(matImage, matImage);
      flip(matImage, matImage,0); //transpose+flip(0)=CCW
      break;
    case 2:
      flip(matImage, matImage,-1); //flip(-1)=180
      break;
    case 3:
      transpose(matImage, matImage);
      flip(matImage, matImage,1); //transpose+flip(1)=CW
      break;
    default:
      std::cout  << "[rot90] Unknown rotation flag(" << k << ")" << std::endl;
      break;
  }
}
void rot90(cv::Mat *matImage, int k){
  // rot 90*k degree counterclockwise
  switch(k) {
    case 0:
      break;
    case 1:
      transpose(*matImage, *matImage);
      flip(*matImage, *matImage,0); //transpose+flip(0)=CCW
      break;
    case 2:
      flip(*matImage, *matImage,-1); //flip(-1)=180
      break;
    case 3:
      transpose(*matImage, *matImage);
      flip(*matImage, *matImage,1); //transpose+flip(1)=CW
      break;
    default:
      std::cout  << "[rot90] Unknown rotation flag(" << k << ")" << std::endl;
      break;
  }
}
/*
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
*/
