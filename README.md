# imgloader

## usage
```c
bmp_t a = imgloader_bmp_load("res/img/bmp/1x1.bmp");

if (a.data == NULL)
{
    printf("%s\n", imgloader_last_error);
    /* error handling */
}

/* ... */

imgloader_free(&a.data);
```

###### everything is early and temp
