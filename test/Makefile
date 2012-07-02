CC = clang
CFLAGS = -Wall -Wextra -std=gnu99 -g `use-cc alist htable`

all: $(patsubst %.c, %, $(wildcard *.c))

clean:
	rm -f basic al ht 
