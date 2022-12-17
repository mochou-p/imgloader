# imgloader
|Structure member|Type|Description|
|:-:|:-:|:-:|
|`.width`|`int`|image width|
|`.height`|`int`|image height|
|`.data`|`unsigned char*`|1D array of image bytes (pixel colors)|

## Pure example
```c
/* C */

img_t my_img = imgloader_load("path/to/my_cute_cat.bmp");

if (my_img.data == NULL)
{
    printf("%s\n", imgloader_last_error); /* short error msg */
    /* ... (error handling) */
}

/* ... (use my_img here) */

imgloader_free(&my_img.data);
```

## OpenGL example
```cpp
// C++

auto my_img = imgloader_load("path/to/my_cute_cat.bmp");

if (my_img.data == nullptr)
{
    std::cout << imgloader_last_error << std::endl; // short error msg
    // ... (error handling)
}

// generate an OpenGL texture
GLuint my_texture;
glGenTextures(1, &my_texture);
glBindTexture(GL_TEXTURE_2D, my_texture);

// required for WebGL if width and height are not 2^n
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

// insert pixels into the texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, my_img.width, my_img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, my_img.data);

imgloader_free(&my_img.data);

// ... (use my_texture here)
```

## Supported image formats
|Name|Extension|
|-|-|
|[Windows BMP Image](https://en.wikipedia.org/wiki/BMP_file_format)|`.bmp`|
|[PPM Image](https://en.wikipedia.org/wiki/Netpbm)|`.pgm` `.ppm`|
|[Truevision TGA](https://en.wikipedia.org/wiki/Truevision_TGA)|`.tga`|

---
###### *unstable temporary implementations
