# imgloader

## usage
```c
img_t img = imgloader_load("res/2x3.bmp");

if (img.data == NULL)
{
    printf("%s\n", imgloader_last_error);
    /* ... (your optional error handling) */
}

/* ... (use img here if loaded successfully)
 *
 * img.data is stored as a stream of colors
 * order is left to right, top to bottom
 * { pixel_1.r, pixel_1.g, pixel_1.b, pixel_2.r, ... }
 */

imgloader_free(&img.data);
```

###### everything is early and temp
