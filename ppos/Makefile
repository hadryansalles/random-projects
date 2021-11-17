#####################################################################
# @file    Makefile
# @brief   Simple makefile to build and run each PPOS test target
# @authors Hadryan Sales    2026961
#          Pyettra Ferreira 2191130
# @created 07/14/2021
#####################################################################

# source files
source=source/ppos-core-aux.c source/disk.c source/ppos_disk.c
# libraries to link with
libs=libs/libppos_static.a
# output dir for binary files
bindir=bin
# c compiler
cc=gcc
# flags passed to the compiler
flags=-Wall -lrt #-DDEBUG

all:
	@echo "usage: make <test> run"
	@echo "list of tests: timer"
	@echo "               signal"
	@echo "               disco1"
	@echo "               disco2"
	@echo "               scheduler"
	@echo "               preempcao"
	@echo "               contab-prio"
	@echo "               preempcao-stress"

# build the test source file
%.o: %.c $(bindir)
	$(cc) $(source) $(libs) $(flags) $< -o $(bindir)/a.out

# run the binary file
run: $(bindir)/a.out
	cd $(bindir) && ./a.out && cd ..

# create a directory for the binary files
$(bindir):
	mkdir $(bindir)

# targets for each test file
timer: source/timer.o
signal: source/signal.o
disco1: source/pingpong-disco1.o
disco2: source/pingpong-disco2.o
scheduler: source/pingpong-scheduler.o
preempcao: source/pingpong-preempcao.o 
contab-prio: source/pingpong-contab-prio.o
preempcao-stress: source/pingpong-preempcao-stress.o

# just clean the binary dir
clean:
	rm -rf $(bindir)
