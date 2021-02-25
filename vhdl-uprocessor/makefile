###############################################################
# file: makefile
# authors: Ricky Habegger and Hadryan Salles
# date: 08/18/2020
# brief: makefile 
###############################################################

# build variables
target=processor_tb
time=360us

# build config 
GHDL=ghdl
WORKDIR=work
WAVEDIR=wave
GHDLFLAGS= --workdir=$(WORKDIR)
SIZE_VIEW=25
SOURCES=$(wildcard ./src/*.vhd ./tests/*.vhd)

ifeq ($(OS),Windows_NT)
	# windows functions for creating and deleting directory
	MKDIR=if not exist $(1) mkdir $(1)
	RMDIR=del /f /s /q $(1) 1>nul & rmdir /s /q $(1)
else
	# linux functions for creating and deleting directory
	MKDIR=mkdir -p $(1)
	RMDIR=rmdir --ignore-fail-on-non-empty $(1) 
endif

all: run
	@echo -e "$(WAVEDIR)/$(target).ghw updated"

import:
	$(call MKDIR, $(WORKDIR)) 
	$(GHDL) -i $(GHDLFLAGS) $(SOURCES)

compile: import
	$(GHDL) -m $(GHDLFLAGS) $(target)

run: compile
	$(call MKDIR, $(WAVEDIR))
	$(call MKDIR, $(WORKDIR))
	$(GHDL) -r $(GHDLFLAGS) $(target)\
		--stop-time=$(time) --wave=$(WAVEDIR)/$(target).ghw

view: run
	gtkwave $(WAVEDIR)/$(target).ghw --save=$(WAVEDIR)/$(target).gtkw\
		--rcvar 'fontname_signals Monospace $(SIZE_VIEW)'\
		--rcvar 'fontname_waves Monospace $(SIZE_VIEW)'

clean:
	ghdl --clean $(GHDLFLAGS)
	$(call RMDIR, $(WORKDIR))
	
remove:
	ghdl --remove $(GHDLFLAGS)
	$(call RMDIR, $(WORKDIR))

test:
	python test_all.py
