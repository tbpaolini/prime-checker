SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:.c=.o)

ifeq ($(OS),Windows_NT)
    EXECUTABLES := $(SOURCES:.c=.exe)
else
    EXECUTABLES := $(SOURCES:.c=)
endif

.PHONY: release debug memcheck all

release: CFLAGS := -O3
release: all

debug: CFLAGS := -g
debug: all

memcheck: CFLAGS := -g -fsanitize=address -fsanitize=leak
memcheck: all

all: $(EXECUTABLES)

ifeq ($(OS),Windows_NT)
    %.exe: %.c
	    gcc $< -o $@ $(CFLAGS)

else
    %: %.c
	    gcc $< -o $@ $(CFLAGS) -lm

endif