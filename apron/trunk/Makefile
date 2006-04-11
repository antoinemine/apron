
include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)
#
PREFIX = $(APRON_PREFIX)
#
# C include and lib directories
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
BINDIR = $(PREFIX)/bin
#

#---------------------------------------
# C part
#---------------------------------------
CC = gcc
ICFLAGS = \
-I$(GMP_PREFIX)/include \
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

NOWEB_FILES = \
ap_general.nw \
ap_coefficients.nw ap_dimension.nw ap_expr0.nw \
ap_manager.nw ap_abstract0.nw \
ap_var.nw ap_environment.nw ap_expr1.nw \
ap_abstract1.nw \
ap_implementorman.nw

TEX_FILES = $(NOWEB_FILES:%.nw=%.tex)
TEX_MAIN = ap.tex

H_FILES = \
ap_config.h \
ap_scalar.h ap_interval.h ap_coeff.h ap_dimension.h \
ap_linexpr0.h ap_lincons0.h ap_generator0.h ap_expr0.h \
ap_manager.h ap_abstract0.h \
ap_var.h ap_environment.h \
ap_linexpr1.h ap_lincons1.h ap_generator1.h ap_expr1.h \
ap_abstract1.h \
ap_global0.h ap_global1.h

C_FILES = \
ap_scalar.c ap_interval.c ap_coeff.c ap_dimension.c \
ap_linexpr0.c ap_lincons0.c ap_generator0.c \
ap_manager.c ap_abstract0.c \
ap_var.c ap_environment.c \
ap_linexpr1.c ap_lincons1.c ap_generator1.c \
ap_abstract1.c

#---------------------------------------
# Rules
#---------------------------------------

all: clib ps

clib: $(H_FILES) libapron.a libapron_debug.a

csrc: $(H_FILES)

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(NOWEB_FILES) $(H_FILES) $(C_FILES) apron.texi apronrationale.tex Makefile README
	(cd ..; tar zcvf $(HOME)/apron.tgz $(^:%=apron/%))

dist: $(NOWEB_FILES) apron.texi apronrationale.tex Makefile README $(TEX_FILES) $(H_FILES) $(C_FILES) apronrationale.dvi
	(cd ..; tar zcvf $(HOME)/apron.tgz $(^:%=apron/%))

clean:
	/bin/rm -f $(NOWEB_FILES:%.nw=%.h) $(TEX_FILES)
	/bin/rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *.ps apron.cps apron.fns apron.info apron.kys apron.pgs apron.tps apron.vrs
	/bin/rm -f *.o *.a *.cmi *.cmo *.cmx *.cmxa *.cma

install: $(H_FILES) libapron.a libapron_debug.a
	mkdir -p $(APRON_PREFIX)/include
	cp $(H_FILES) $(APRON_PREFIX)/include
	mkdir -p $(APRON_PREFIX)/lib
	cp libapron.a libapron_debug.a $(APRON_PREFIX)/lib

distclean:
	/bin/rm -f $(H_FILES:%=$(APRON_PREFIX)/include/%)
	/bin/rm -f $(APRON_PREFIX)/lib/libapron.a
	/bin/rm -f $(APRON_PREFIX)/lib/libapron_debug.a

#---------------------------------------
# Latex rules
#---------------------------------------

.PHONY : dvi apronrationale.dvi 

ps: apronrationale.dvi
	$(DVIPS) apronrationale -o

dvi: apronrationale.tex $(TEX_FILES)
	$(LATEX) apronrationale.tex

apronrationale.dvi: apronrationale.tex $(TEX_FILES)
	$(LATEX) $^

#---------------------------------------
# C rules
#---------------------------------------

libapron.a: $(C_FILES:%.c=%.o)
	ar rcs $@ $^
libapron_debug.a: $(C_FILES:%.c=%_debug.o)
	ar rcs $@ $^

.PHONY : dep

cdep: $(H_FILES)
	for i in $(C_FILES); do			\
		cpp -MM $$i;			\
	done

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .tex .fig .c .h .o _debug.o _prof.o .nw

#---------------------------------------
# Noweb generic rules
#---------------------------------------

%.h: %.nw
	notangle -L'#line %L "$(SRCDIR)/%F"%N' $^ >$@
#	notangle $^ >$@
%.tex: %.nw
	noweave -n $^ >$@

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c %.h 
	$(CC) $(CFLAGS) -c $<

%_debug.o: %.c %.h $(H_FILES)
	$(CC) $(CFLAGS_DEBUG) -c -o $@ $<

#---------------------------------------
# dependencies (generated with make dep)
#---------------------------------------

ap_scalar.o: ap_scalar.c ap_scalar.h ap_config.h
ap_interval.o: ap_interval.c ap_interval.h ap_config.h ap_scalar.h
ap_coeff.o: ap_coeff.c ap_coeff.h ap_config.h ap_scalar.h ap_interval.h
ap_dimension.o: ap_dimension.c ap_dimension.h
ap_linexpr0.o: ap_linexpr0.c ap_linexpr0.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_dimension.h
ap_lincons0.o: ap_lincons0.c ap_lincons0.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_linexpr0.h ap_dimension.h
ap_generator0.o: ap_generator0.c ap_generator0.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_linexpr0.h ap_dimension.h
ap_manager.o: ap_manager.c ap_manager.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h
ap_abstract0.o: ap_abstract0.c ap_abstract0.h ap_manager.h ap_coeff.h \
  ap_config.h ap_scalar.h ap_interval.h ap_expr0.h ap_linexpr0.h \
  ap_dimension.h ap_lincons0.h ap_generator0.h
ap_var.o: ap_var.c ap_manager.h ap_coeff.h ap_config.h ap_scalar.h \
  ap_interval.h ap_var.h
ap_environment.o: ap_environment.c ap_manager.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_environment.h ap_dimension.h ap_var.h
ap_linexpr1.o: ap_linexpr1.c ap_linexpr1.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_environment.h ap_dimension.h ap_var.h \
  ap_linexpr0.h
ap_lincons1.o: ap_lincons1.c ap_lincons1.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_environment.h ap_dimension.h ap_var.h \
  ap_lincons0.h ap_linexpr0.h ap_linexpr1.h
ap_generator1.o: ap_generator1.c ap_generator1.h ap_coeff.h ap_config.h \
  ap_scalar.h ap_interval.h ap_environment.h ap_dimension.h ap_var.h \
  ap_generator0.h ap_linexpr0.h ap_linexpr1.h
ap_abstract1.o: ap_abstract1.c ap_coeff.h ap_config.h ap_scalar.h \
  ap_interval.h ap_manager.h ap_abstract0.h ap_expr0.h ap_linexpr0.h \
  ap_dimension.h ap_lincons0.h ap_generator0.h ap_expr1.h ap_linexpr1.h \
  ap_environment.h ap_var.h ap_lincons1.h ap_generator1.h ap_abstract1.h
