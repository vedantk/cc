CC = clang
CFLAGS = -Wall -Wextra -std=gnu99 -O2 `use-cc AL HT POOL`

CXX = clang++
CXXFLAGS = -Wall -Wextra -O2 `use-cc`

all: $(patsubst %.c, %, $(wildcard *.c)) torture-map

torture-map: torture-map.cc

clean:
	rm -f basic al ht pool cval torture-ht torture-map
