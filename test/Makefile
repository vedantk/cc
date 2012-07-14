CC = clang
CFLAGS = -Wall -Wextra -std=gnu99 -g `use-cc AL HT POOL`

all: $(patsubst %.c, %, $(wildcard *.c))

clean:
	rm -f basic al ht pool cval
