/**/

#include "imgloader.h"

int main(void)
{
    bmp_t a = bmp_load("res/2xs1.bmp");
    printf(".id     = %s\n.width  = %d\n.height = %d\n.bits   = %d\n", a.id, a.width, a.height, a.bits);

    return 0;
}
