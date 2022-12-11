/* currently works properly only for 24-bit bmps */

#pragma once

#ifndef __imgloader_h_
#define __imgloader_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMGLOADER_BMP_OFFSET_ID          0x00
#define IMGLOADER_BMP_OFFSET_WIDTH       0x12
#define IMGLOADER_BMP_OFFSET_HEIGHT      0x16
#define IMGLOADER_BMP_OFFSET_BITS        0x1C
#define IMGLOADER_BMP_OFFSET_COMPRESSION 0x1E
#define IMGLOADER_BMP_OFFSET_DATA        0x36

#define IMGLOADER_BMP_COMPRESSION_NO     0x00

typedef struct bmp
{
    char           id[3];
    int            width;
    int            height;
    int            bits;
    unsigned char* data;
} bmp_t;

static char imgloader_last_error[24] = "no error";

static void imgloader_free(unsigned char** _data)
{
    if (*_data == NULL)
        return;

    free(*_data);
    *_data = NULL;
}

static bmp_t imgloader_bmp_load(const char* _filepath)
{
    bmp_t          b;
    FILE*          f;
    unsigned char* buf;
    long           len;
    int            size, pad, ofs, x, y;

    buf    = NULL;
    b.data = NULL;

    if ((f = fopen(_filepath, "rb")) == NULL)
    {
        strcpy(imgloader_last_error, "fopen fail");
        goto error;
    }

    if (fseek(f, 0, SEEK_END))
    {
        strcpy(imgloader_last_error, "fseek fail");
        goto error;
    }

    if ((len = ftell(f)) < 0)
    {
        strcpy(imgloader_last_error, "ftell fail");
        goto error;
    }

    rewind(f);

    if ((buf = (unsigned char*) malloc(sizeof(unsigned char) * len)) == NULL)
    {
        strcpy(imgloader_last_error, "malloc fail");
        goto error;
    }

    if (fread(buf, len, 1, f) != 1)
    {
        strcpy(imgloader_last_error, "fread fail");
        goto error;
    }

    if (buf[IMGLOADER_BMP_OFFSET_COMPRESSION] != IMGLOADER_BMP_COMPRESSION_NO)
    {
        strcpy(imgloader_last_error, "unknown bmp compression");
        goto error;
    }

    b.id[0]  = buf[IMGLOADER_BMP_OFFSET_ID    ];
    b.id[1]  = buf[IMGLOADER_BMP_OFFSET_ID + 1];
    b.id[2]  = '\0';
    b.width  = buf[IMGLOADER_BMP_OFFSET_WIDTH ];
    b.height = buf[IMGLOADER_BMP_OFFSET_HEIGHT];
    b.bits   = buf[IMGLOADER_BMP_OFFSET_BITS  ];

    size     = b.width * b.height * 3;
    b.data   = (unsigned char*) malloc(sizeof(unsigned char) * size);

    pad      = 4 * (b.width % 4);
    ofs      = IMGLOADER_BMP_OFFSET_DATA;

    for (y = b.height - 1; y >= 0; --y)
    {
        for (x = b.width - 1; x >= 0; --x)
        {
            b.data[--size] = buf[ofs + x*3    ];
            b.data[--size] = buf[ofs + x*3 + 1];
            b.data[--size] = buf[ofs + x*3 + 2];
        }

        ofs += pad;
    }

error:
    if (f != NULL && fclose(f))
        strcpy(imgloader_last_error, "fclose fail");

    if (buf != NULL)
        free(buf);

    return b;
}

#endif /* __imgloader_h_ */
