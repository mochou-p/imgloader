/**/

#pragma once
#ifndef __imgloader_h_
#define __imgloader_h_

#include <stdio.h>
#include <stdlib.h>

#define IMGLOADER_BMP_ID_0   0x00
#define IMGLOADER_BMP_ID_1   0x01
#define IMGLOADER_BMP_WIDTH  0x12
#define IMGLOADER_BMP_HEIGHT 0x16
#define IMGLOADER_BMP_BITS   0x1C
#define IMGLOADER_BMP_OFFSET 0x36

typedef struct bmp
{
    char  id[3];
    int   width;
    int   height;
    int   bits;
    char* data;
} bmp_t;

static void imgloader_free(char** _data)
{
    if (*_data == NULL)
        return;

    free(*_data);
    *_data = NULL;
}

bmp_t imgloader_bmp_load(const char* _filepath)
{
    bmp_t  b;
    FILE*  f;
    char*  buf;
    long   len;

    b.data = NULL;

    if ((f = fopen(_filepath, "rb")) == NULL)
    {
#ifdef IMGLOADER_LOG
        puts("fopen fail");
#endif
        goto exit;
    }

    if (fseek(f, 0, SEEK_END))
    {
#ifdef IMGLOADER_LOG
        puts("fseek fail");
#endif
        goto fclose_exit;
    }

    if ((len = ftell(f)) < 0)
    {
#ifdef IMGLOADER_LOG
        puts("ftell fail");
#endif
        goto fclose_exit;
    }

    rewind(f);

    if ((buf = (char*) malloc(sizeof(char) * len)) == NULL)
    {
#ifdef IMGLOADER_LOG
        puts("malloc fail");
#endif
        goto fclose_exit;
    }

    if (fread(buf, len, 1, f) != 1)
    {
#ifdef IMGLOADER_LOG
        puts("fread fail");
#endif
        goto fclose_exit;
    }

    b.id[0]  = buf[IMGLOADER_BMP_ID_0  ];
    b.id[1]  = buf[IMGLOADER_BMP_ID_1  ];
    b.id[2]  = '\0';
    b.width  = buf[IMGLOADER_BMP_WIDTH ];
    b.height = buf[IMGLOADER_BMP_HEIGHT];
    b.bits   = buf[IMGLOADER_BMP_BITS  ];

    b.data = (char*) malloc(sizeof(char) * b.width * b.height);

    /* i % 4 == 0 ? i : i + 4 - i % 4 */

    free(buf);

    return b;

fclose_exit:
    if (fclose(f))
    {
#ifdef IMGLOADER_LOG
        puts("fclose fail");
#endif
    }

exit:
    return b;
}

#endif /* __imgloader_h_ */
