#include "PuissanceN.h"

int main()
{
  Image* Bitmap = load_image("possion.jpg");
  BalanceAbsolue(Bitmap,0.01);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
