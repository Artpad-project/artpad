#include "DetectionCoin.h"

int main()
{
  Image* Bitmap = load_image("Test/gruik2.jpg");
  Detection(Bitmap);
  //save_image(Bitmap,"result","png");
  free_image(Bitmap);
  return 0;
}
