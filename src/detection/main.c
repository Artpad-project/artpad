#include "DetectionCoin.h"

int main()
{
  Image* Bitmap = load_image("Test/retinex.png");
  Detection(Bitmap,1.0,2.5,0.006,130);
  //save_image(Bitmap,"result","png");
  free_image(Bitmap);
  free(Bitmap);
  return 0;
}
