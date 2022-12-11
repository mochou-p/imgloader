#

CC     := gcc -std=c89 -O3 -g3
CFLAGS := -Wall -Wextra -Wold-style-definition -Werror

test:
	$(CC) $(CFLAGS) $@.c -o $@.exe
	- .\$@ ||:
