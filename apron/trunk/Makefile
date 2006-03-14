#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)
#
# Installation directory prefix
COMMONINTERFACE_INSTALL = $(HOME)/pkg/commoninterface/$(ARCH)
#
PREFIX = $(COMMONINTERFACE_INSTALL)
#
# C include and lib directories
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
BINDIR = $(PREFIX)/bin
#
# Where to find necessary files
# GMP include and lib directories
# GMP_INSTALL =

NUM_INSTALL = $(HOME)/pkg/num

#---------------------------------------
# C part
#---------------------------------------
CC = gcc
ICFLAGS = \
-I/udd/bjeannet/dev/num \
-I$(GMP_INSTALL)/include \
-Wall -Winline -Wimplicit-function-declaration -std=c99 

CFLAGS = $(ICFLAGS) -O3 -DNDEBUG
CFLAGS_DEBUG = $(ICFLAGS) -O0 -g -UNDEBUG
CFLAGS_PROF = $(CFLAGS) -g -pg

#---------------------------------------
# Latex part
#---------------------------------------
LATEX=latex
DVIPS=dvips

#---------------------------------------
# Files
#---------------------------------------

NOWEB_FILES = general.nw manager.nw coeff.nw expr0.nw abstract0.nw environment.nw expr1.nw abstract1.nw implementorman.nw
TEX_FILES = $(NOWEB_FILES:%.nw=%.tex)
TEX_MAIN = commoninterface.tex

H_FILES = manager.h coeff.h linexpr0.h lincons0.h generator0.h expr0.h abstract0.h environment.h expr1.h abstract1.h
C_FILES = manager.c coeff.c linexpr0.c lincons0.c generator0.c environment.c expr1.c abstract1.c

#---------------------------------------
# Rules
#---------------------------------------

all: ps C

C: $(H_FILES) libci.a libci_debug.a

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(NOWEB_FILES) commoninterface.tex Makefile
	(cd ..; tar zcvf $(HOME)/commoninterface.tgz $(^:%=commoninterface/%))

dist: $(NOWEB_FILES) commoninterface.tex Makefile $(TEX_FILES) $(H_FILES) $(C_FILES) commoninterface.dvi 
	(cd ..; tar zcvf $(HOME)/commoninterface.tgz $(^:%=commoninterface/%))

clean: 
	/bin/rm -f $(NOWEB_FILES:%.nw=%.h) $(TEX_FILES)
	/bin/rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *.ps
	/bin/rm -f *.o *.a *.cmi *.cmo *.cmx *.cmxa *.cma 

install: $(H_FILES) libci.a libci_debug.a
	mkdir -p $(COMMONINTERFACE_INSTALL)/include
	cp $(H_FILES) $(COMMONINTERFACE_INSTALL)/include
	mkdir -p $(COMMONINTERFACE_INSTALL)/lib
	cp libci.a libci_debug.a $(COMMONINTERFACE_INSTALL)/lib

distclean:
	/bin/rm -f $(H_FILES:%=$(COMMONINTERFACE_INSTALL)/include/%)
	/bin/rm -f $(COMMONINTERFACE_INSTALL)/lib/libci.a
	/bin/rm -f $(COMMONINTERFACE_INSTALL)/lib/libci_debug.a

#---------------------------------------
# Latex rules
#---------------------------------------
ps: commoninterface.dvi
	$(DVIPS) commoninterface -o

dvi: commoninterface.tex $(TEX_FILES)
	$(LATEX) commoninterface.tex

commoninterface.dvi: commoninterface.tex $(TEX_FILES)
	$(LATEX) $^

#---------------------------------------
# C rules
#---------------------------------------

libci.a: $(C_FILES:%.c=%.o)
	ar rcs $@ $^
libci_debug.a: $(C_FILES:%.c=%_debug.o)
	ar rcs $@ $^


#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .tex .fig .c .h .o _debug.o _prof.o .nw

#---------------------------------------
# Noweb generic rules
#---------------------------------------

%.h: %.nw
	notangle -L'#line %L "$(SRCDIR)/%F"%N' -R$@ $^ >$@
#	notangle -R$@ $^ >$@
%.tex: %.nw
	noweave -n $^ >$@

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c %.h $(H_FILES)
	$(CC) $(CFLAGS) -c $<

%_debug.o: %.c %.h $(H_FILES)
	$(CC) $(CFLAGS_DEBUG) -c -o $@ $<

