CC  = gcc
OBJ	= o
EXE	= out
RUN     =
CCFLAGS  = 
UNAME := $(shell uname -a)
ifeq ($(OS),Windows_NT)
	OBJ	= obj
	EXE	= exe
endif

all: build run

build: generator.c
	$(CC) $(CCFLAGS)  -o generator.$(EXE) $<

run: generator.$(EXE)
	$(RUN) ./$<

clean:
	@echo 'Cleaning up...'
	@rm -rf *.$(EXE) *.$(OBJ) *.dwf *.pdb prof
