#pragma once

#ifndef __imgloader_h_
#define __imgloader_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMGLOADER_BMP_OFFSET_WIDTH  0x12
#define IMGLOADER_BMP_OFFSET_HEIGHT 0x16
#define IMGLOADER_BMP_OFFSET_DATA   0x36

#define IMGLOADER_EXT_BMP           7368034L

typedef struct img
{
    int            width;
    int            height;
    unsigned char* data;
} img_t;

static unsigned char* read_file          (const    char* );
static void           imgloader_free     (unsigned char**);
static img_t          imgloader_load     (const    char* );
static img_t          imgloader_bmp_load (unsigned char* );

static char imgloader_last_error[24] = "no error";

static void imgloader_free(unsigned char** _data)
{
    if (*_data == NULL)
        return;

    free(*_data);
    *_data = NULL;
}

static img_t imgloader_load(const char* _filepath)
{
    unsigned char* buf;
    long           ext;

    if ((buf = read_file(_filepath)) == NULL)
        goto error;

    ext = *((long*) (strchr(_filepath, '.')+1));

    switch (ext)
    {
    case IMGLOADER_EXT_BMP:
        return imgloader_bmp_load(buf);
    default:
        break;
    }

    free(buf);

error:
    img_t bad_img = { .data = NULL };
    return bad_img;
}

static unsigned char* read_file(const char* _filepath)
{
    FILE*          f;
    unsigned char* buf;
    long           len;

    buf = NULL;

    if ((f = fopen(_filepath, "rb")) == NULL)
    {
        strcpy(imgloader_last_error, "fopen fail");
        goto out;
    }

    if (fseek(f, 0, SEEK_END))
    {
        strcpy(imgloader_last_error, "fseek fail");
        goto out;
    }

    if ((len = ftell(f)) < 0)
    {
        strcpy(imgloader_last_error, "ftell fail");
        goto out;
    }

    rewind(f);

    if ((buf = (unsigned char*) malloc(sizeof(unsigned char) * len)) == NULL)
    {
        strcpy(imgloader_last_error, "malloc fail");
        goto out;
    }

    if (fread(buf, len, 1, f) != 1)
    {
        strcpy(imgloader_last_error, "fread fail");
        imgloader_free(&buf);
    }

#ifdef PRINT_FILE
    int i = 0;

    for (; i < len; i++)
        printf("%u ", buf[i]);

    printf("\n");
#endif

out:
    if (f != NULL && fclose(f))
        strcpy(imgloader_last_error, "fclose fail");

    return buf;
}

static img_t imgloader_bmp_load(unsigned char* _buf)
{
    img_t img;
    int   size, pad, ofs, x, y;

    ofs        = IMGLOADER_BMP_OFFSET_DATA;

    img.width  = *((int*) (_buf+IMGLOADER_BMP_OFFSET_WIDTH ));
    img.height = *((int*) (_buf+IMGLOADER_BMP_OFFSET_HEIGHT));

    pad        = 4 * (img.width % 4);
    size       = img.width * img.height * 3;

    img.data   = (unsigned char*) malloc(sizeof(unsigned char) * size);

    for (y = img.height - 1; y >= 0; --y)
    {
        for (x = img.width - 1; x >= 0; --x)
        {
            img.data[--size] = _buf[ofs + x*3    ];
            img.data[--size] = _buf[ofs + x*3 + 1];
            img.data[--size] = _buf[ofs + x*3 + 2];
        }

        ofs += pad;
    }

    return img;
}

#endif /* __imgloader_h_ */
