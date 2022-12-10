#

CC     := gcc -std=c89 -O3 -g3
CFLAGS := -Wall -Wextra -Wold-style-definition -Werror

_:
	$(CC) $(CFLAGS) main.c -o main.exe
	.\main
	@ del main.exe 2>nul
