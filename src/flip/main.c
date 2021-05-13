#include "../../include/Flip.h"

int main()
{
  Image* Bitmap = load_image("Test/yiff.jpg");
  FlipHorizon(Bitmap);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
