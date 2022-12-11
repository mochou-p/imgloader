# imgloader

## usage
```c
bmp_t my_bmp = imgloader_bmp_load("res/img/bmp/1x1.bmp");

if (my_bmp.data == NULL)
{
    printf("%s\n", imgloader_last_error);
    /* error handling */
}

/* use my_bmp here
 *
 * my_bmp.data is stored as a stream of colors
 * order is left to right, top to bottom
 * { pixel_1.r, pixel_1.g, pixel_1.b, pixel_2.r, ... }
 */

imgloader_free(&my_bmp.data);
```

###### everything is early and temp
