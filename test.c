/**/

#include "imgloader.h"

int main(void)
{
    int i;
    bmp_t a = imgloader_bmp_load("res/img/bmp/1x1.bmp");
    
    if (a.data == NULL)
    {
        printf("%s\n", imgloader_last_error);
        return 1;
    }

    printf
    (
        ".id     = %s\n.width  = %d\n.height = %d\n.bits   = %d\n",
        a.id, a.width, a.height, a.bits
    );

    printf("\n| ");
    for (i = 0; i < a.width * a.height * 3; ++i)
    {
        printf("%02d ", a.data[i]);
        if ((i+1) % 3 == 0)
            printf("| ");
    }

    imgloader_free(&a.data);

    return 0;
}
