#include "DetectionCoin.h"
#include "KLT.h"

int main()
{
  int*  nbPoint = malloc(sizeof(int));
  *nbPoint = 0;
  Coord* CoordList;
  Image* Bitmap1 = load_image("frames/out-006.jpg");
  Image* Bitmap2 = load_image("frames/out-008.jpg");
  CoordList = Detection(Bitmap1,1.0,2.5,0.006,250,nbPoint);
  KLT(Bitmap1,Bitmap2,4,CoordList,*nbPoint);
  save_image(Bitmap1,"result1","jpeg");
  save_image(Bitmap2,"result2","jpeg");
  free_image(Bitmap1);
  free_image(Bitmap2);
  free(Bitmap1);
  free(Bitmap2);
  free(nbPoint);
  free(CoordList);
  return 0;
}
