#include "../../include/Rotation.h"

int main()
{
  Image* Bitmap = load_image("Test/bez.jpg");
  Rotate(Bitmap,45);
  save_image(Bitmap,"result","jpeg");
  free_image(Bitmap);
  return 0;
}
