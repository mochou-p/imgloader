/**/

#pragma once

#ifndef __imgloader_h_
#define __imgloader_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMGLOADER_EXTHASH_BMP 1230794
#define IMGLOADER_EXTHASH_PGM 1292828
#define IMGLOADER_EXTHASH_PPM 1404698

typedef struct img
{
    int            width;
    int            height;
    unsigned char* data;
} img_t;

static int            hash           (const          char* );
static unsigned char* read_file      (const          char* );
static void           imgloader_free (unsigned       char**);
static img_t          imgloader_load (const          char* );
static img_t          parse_bmp_file (const unsigned char* );
static img_t          parse_pgm_file (const unsigned char* );
static img_t          parse_ppm_file (const unsigned char* );

static char imgloader_last_error[24] = "no error";

static int hash(const char* _str)
{
    int h, i;

    h = 0; i = -1;

    while (_str[++i])
        h += _str[i]*(i+h)+_str[i]/(h+1);

    return h;
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

#ifdef DEBUG
    int i = 0;
    printf("%s bytes:\n", _filepath);
    for (; i < len; i++)
        printf("%u ", buf[i]);
    printf("\n\n");
#endif

out:
    if (f != NULL && fclose(f))
        strcpy(imgloader_last_error, "fclose fail");

    return buf;
}

static void imgloader_free(unsigned char** _data)
{
    if (*_data == NULL)
        return;

    free(*_data);
    *_data = NULL;
}

static img_t imgloader_load(const char* _filepath)
{
    img_t          img;
    unsigned char* buf;
    char*          ext;

    img.data = NULL;

    if ((buf = read_file(_filepath)) == NULL)
        return img;

    ext = strchr(_filepath, '.')+1;

#ifdef DEBUG
    printf("%s extension hash:\n%d\n\n", ext, hash(ext));
#endif

    switch (hash(ext))
    {
    case IMGLOADER_EXTHASH_BMP:
        img = parse_bmp_file(buf);
        break;
    case IMGLOADER_EXTHASH_PGM:
        img = parse_pgm_file(buf);
        break;
    case IMGLOADER_EXTHASH_PPM:
        img = parse_ppm_file(buf);
        break;
    default:
        strcpy(imgloader_last_error, "unknown image format");
        break;
    }

    free(buf);

    return img;
}

static img_t parse_bmp_file(const unsigned char* _buf)
{
    img_t img;
    int   size, pad, ofs, x, y;

    ofs        = 54;
    img.width  = *((int*) (_buf+18));
    img.height = *((int*) (_buf+22));
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

static img_t parse_pgm_file(const unsigned char* _buf)
{
    img_t img;
    int   i, j, k, l, size, end, ofs, fact;
    char  width[6], height[6], maxval[4];

    i = 0; j = 0; k = 0; l = 0;

    while (_buf[++i] != '\n')
        ;

    while (_buf[++i] != ' ')
        width[j++]  = _buf[i];

    while (_buf[++i] != '\n')
        height[k++] = _buf[i];

    while (_buf[++i] != '\n')
        maxval[l++] = _buf[i];

    width[j]   = '\0';
    height[k]  = '\0';
    maxval[l]  = '\0';
    img.width  = atoi(width);
    img.height = atoi(height);
    fact       = atoi(maxval);
    size       = img.width * img.height * 3;
    end        = i + j + k + size - 1;

    img.data   = (unsigned char*) malloc(sizeof(unsigned char) * size);

    for (i = 0, ofs = end - size; ofs < end; ++ofs)
    {
        img.data[i++] = _buf[ofs]*255/fact;
        img.data[i++] = _buf[ofs]*255/fact;
        img.data[i++] = _buf[ofs]*255/fact;
    }

    return img;
}

static img_t parse_ppm_file(const unsigned char* _buf)
{
    img_t img;
    int   i, j, k, l, size, end, ofs, fract;
    char  width[6], height[6], maxval[4];

    i = 0; j = 0; k = 0; l = 0;

    while (_buf[++i] != '\n')
        ;

    while (_buf[++i] != ' ')
        width[j++]  = _buf[i];

    while (_buf[++i] != '\n')
        height[k++] = _buf[i];

    while (_buf[++i] != '\n')
        maxval[l++] = _buf[i];

    width[j]   = '\0';
    height[k]  = '\0';
    maxval[l]  = '\0';
    img.width  = atoi(width);
    img.height = atoi(height);
    fract      = atoi(maxval);
    size       = img.width * img.height * 3;
    end        = i + j + k + size - 1;

    img.data   = (unsigned char*) malloc(sizeof(unsigned char) * size);

    for (i = 0, ofs = end - size; ofs < end; ++ofs)
        img.data[i++] = _buf[ofs]*255/fract;

    return img;
}

#endif /* __imgloader_h_ */
