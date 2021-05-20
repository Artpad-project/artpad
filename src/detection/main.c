#include "DetectionCoin.h"
#include "KLT.h"

int main()
{
  int*  nbPoint;
  Coord* CoordList;
  Image* Bitmap1 = load_image("Test/croix2.jpg");
  Image* Bitmap2 = load_image("Test/croix2.jpg");
  CoordList = Detection(Bitmap1,1.0,2.5,0.006,130,nbPoint);
  KLT(Bitmap1,Bitmap2,5,CoordList,nbPoint);
  free_image(Bitmap1);
  free_image(Bitmap2);
  free(Bitmap1);
  free(Bitmap2);
  return 0;
}
