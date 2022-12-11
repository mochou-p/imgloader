/**/

#define IMGLOADER_LOG
#include "imgloader.h"

int main(void)
{
    int i = 0;
    bmp_t a = imgloader_bmp_load("res/2x3.bmp");

    if (a.data == NULL)
        return 1;

    printf
    (
        ".id     = %s\n.width  = %d\n.height = %d\n.bits   = %d\n",
        a.id, a.width, a.height, a.bits
    );

    for (; i < a.width * a.height * 3; ++i)
    {
        printf("%02d ", a.data[i]);
    }

    imgloader_free(&a.data);

    return 0;
}
