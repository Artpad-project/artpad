#include "../../include/BalanceAuto.h"

int main()
{
  Image* Bitmap = load_image("Test/yiff.jpg");
  BalanceAuto(Bitmap);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
