CC = clang
CFLAGS = -Wall -Wextra -std=gnu99 -I. -Os
SCAN = SCA_VM_OPTS="" sourceanalyzer
SCANFLAGS = -debug -verbose 

BASE_OBJ = /usr/local/lib/cc
BASE_INC = /usr/local/include/cc
USE_CC_DIR = /usr/local/bin

all: $(patsubst %.c, %.o, $(wildcard *.c))

install:
	mkdir -p $(BASE_OBJ)
	cp *.o $(BASE_OBJ)
	mkdir -p $(BASE_INC)
	cp *.h $(BASE_INC)
	mkdir -p $(USE_CC_DIR)
	cp use-cc $(USE_CC_DIR)
	sed -i 's|base_obj|"$(BASE_OBJ)"|g' $(USE_CC_DIR)/use-cc
	sed -i 's|base_inc|"$(BASE_INC)"|g' $(USE_CC_DIR)/use-cc
	chmod -R +rwx $(BASE_OBJ) $(BASE_INC)

uninstall:
	rm -rf $(BASE_OBJ)
	rm -rf $(BASE_INC)
	rm -f $(USE_CC_DIR)/use-cc

clean: 
	rm -f *.o

scan:
	$(SCAN) -b cc -clean
	$(SCAN) $(SCANFLAGS) -b cc gcc $(CFLAGS) -c $(wildcard *.c)
	$(SCAN) $(SCANFLAGS) -b cc -scan
