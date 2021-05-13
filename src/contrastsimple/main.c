#include "../../include/ContrastSimple.h"

int main()
{
  Image* Bitmap = load_image("Test/cbo.jpeg");
  Contrast(Bitmap,0.1,50);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  free(Bitmap);
  return 0;
}
