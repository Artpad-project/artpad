#include "ContrastSimple.h"

int main()
{
  Image* Bitmap = load_image("Test/bez.jpg");
  Contrast(Bitmap,1);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
