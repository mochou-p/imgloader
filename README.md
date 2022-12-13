# imgloader

## Example usage
```c
img_t img = imgloader_load("res/2x3.bmp");

if (img.data == NULL)
{
    printf("%s\n", imgloader_last_error); /* short error msg */
    /* ... (your optional error handling) */
}

/* ... (use img.width, img.height and img.data here)
 *
 * img.data is stored as a 1D array of color values (0-255)
 * order is RGB, left to right, top to bottom
 * { pixel[0][0].r, pixels[0][0].g, pixels[0][0].b, pixels[0][1].r, ... }
 */

imgloader_free(&img.data);
```

## Supported image formats
|Name|Extension|
|-|-|
|[Windows BMP Image](https://en.wikipedia.org/wiki/BMP_file_format#Example_1)|`.bmp`|
|[PPM Image](https://en.wikipedia.org/wiki/Netpbm#PPM_example)|`.ppm`|

---
###### *mostly early temporary implementations at the moment
###### *more image formats soon
