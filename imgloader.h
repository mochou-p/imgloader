/**/

#pragma once
#ifndef __imgloader_h_
#define __imgloader_h_

#include <stdio.h>
#include <stdlib.h>

#define ANSI_RED   "\x1b[31m"
#define ANSI_RESET "\x1b[0m"

#define BMP_ID_0   0x00
#define BMP_ID_1   0x01
#define BMP_WIDTH  0x12
#define BMP_HEIGHT 0x16
#define BMP_BITS   0x1C
#define BMP_OFFSET 0x36

typedef struct bmp
{
    char id[2];
    int  width;
    int  height;
    int  bits;
    int  first;
} bmp_t;

void err(const char* _msg)
{
    fprintf(stderr, "%serror:%s %s fail\n", ANSI_RED, ANSI_RESET, _msg);
    exit(1);
}

bmp_t bmp_load(const char* _filepath)
{
    bmp_t b = {};
    FILE* f;
    char* buf;
    long  len;

    if ((f = fopen(_filepath, "rb")) == NULL)
        err("fopen");

    if (fseek(f, 0, SEEK_END) != 0)
        err("fseek");

    if ((len = ftell(f)) < 0)
        err("ftell");

    rewind(f);

    if ((buf = (char*) malloc(len * sizeof(char))) == NULL)
        err("malloc");

    if (fread(buf, len, 1, f) != 1)
        err("fread");

    if (fclose(f))
        err("fclose");

    b.id[0]  = buf[BMP_ID_0  ];
    b.id[1]  = buf[BMP_ID_1  ];
    b.width  = buf[BMP_WIDTH ];
    b.height = buf[BMP_HEIGHT];
    b.bits   = buf[BMP_BITS  ];
    b.first  = buf[BMP_OFFSET];

    free(buf);

    return b;
}

#endif /* __imgloader_h_ */
