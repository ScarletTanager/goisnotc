INCLUDE_PATHS=/usr/local/homebrew/include
LINKER_PATHS=/usr/local/homebrew/lib

ifndef CC
CC=gcc
endif

LINKER=$(CC)
CGREEN_LIB=-lcgreen

ifdef DEBUG
ifdef DEBUG_GDB
DEBUG_FLAG=-ggdb
else
DEBUG_FLAG=-g
endif
else
DEBUG_FLAG=
OPTIMIZER_FLAG=-O3
endif

COMPILE_CMD=$(CC) -I $(INCLUDE_PATHS) $(DEBUG_FLAG) $(OPTIMIZER_FLAG) -c
LINK_CMD=$(LINKER) -L $(LINKER_PATHS)

all: clean build

build: first_test all_tests main

main.o:
	$(COMPILE_CMD) main.c

benchmark.o:
	$(COMPILE_CMD) benchmark.c

append.o:
	$(COMPILE_CMD) append.c

main: main.o benchmark.o append.o
	$(LINK_CMD) benchmark.o append.o main.o -o foo

first_test:
	$(COMPILE_CMD) first_test.c
	$(LINK_CMD) $(CGREEN_LIB) first_test.o -o first_test

words:
	$(COMPILE_CMD) words.c

words_test: words
	$(COMPILE_CMD) words_test.c

all_tests: words_test
	$(COMPILE_CMD) all_tests.c
	$(LINK_CMD) $(CGREEN_LIB) words.o words_test.o all_tests.o -o all_tests

clean:
	rm -f *.o
	rm -f first_test all_tests foo
