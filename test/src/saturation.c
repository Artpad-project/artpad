#include "../../include/Saturation.h"

int main()
{
  Image* Bitmap = load_image("Test/bez.jpg");
  SaturationAbsolue(Bitmap,20000000);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
