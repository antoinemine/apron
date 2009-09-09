# Makefile
#
# APRON Library / C++ class wrappers
#
# Copyright (C) Antoine Mine' 2007


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
SRCDIR = $(shell pwd)


#---------------------------------------
# Programs
#---------------------------------------

# Library creation
SHARED = gcc -shared


#---------------------------------------
# Flags
#---------------------------------------

# Machine-independent compilation flags.
ICXXFLAGS = \
-I../apron \
-I../num \
-I../itv \
-I../newpolka \
-I../octagons \
-I../box \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

LIBS = \
 -L. -lapronxx_debug \
 -L../newpolka -lpolkaMPQ_debug \
 -L../octagons -loctMPQ_debug \
 -L../box -lboxMPQ_debug

ifneq ($(HAS_PPL),)
DEFS := $(DEFS) -DHAS_PPL
LIBS := $(LIBS) -L$(PPL_PREFIX)/lib -L../ppl -L../products -lap_pkgrid -lap_ppl -lppl
ICXXFLAGS := $(ICXXFLAGS) -I$(PPL_PREFIX)/include  -I../ppl -I../products
endif

LIBS := $(LIBS) \
 -L../apron -lapron_debug \
 -L../itv -litv_debug \
 -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -lgmpxx -lmpfr -lgmp -lm

#---------------------------------------
# Files
#---------------------------------------

CXXSOURCES = apronxx.cc
CCINC = \
 apronxx.hh                  apxx_generator1.hh         apxx_polka_inline.hh		\
 apxx_abstract0.hh           apxx_generator1_inline.hh  apxx_ppl.hh			\
 apxx_abstract0_inline.hh    apxx_interval.hh           apxx_ppl_inline.hh		\
 apxx_abstract1.hh           apxx_interval_inline.hh    apxx_ppl_product.hh		\
 apxx_abstract1_inline.hh    apxx_lincons0.hh           apxx_ppl_product_inline.hh	\
 apxx_box.hh                 apxx_lincons0_inline.hh    apxx_scalar.hh			\
 apxx_box_inline.hh          apxx_lincons1.hh           apxx_scalar_inline.hh		\
 apxx_coeff.hh               apxx_lincons1_inline.hh    apxx_tcons0.hh			\
 apxx_coeff_inline.hh        apxx_linexpr0.hh           apxx_tcons0_inline.hh		\
 apxx_dimension.hh           apxx_linexpr0_inline.hh    apxx_tcons1.hh			\
 apxx_dimension_inline.hh    apxx_linexpr1.hh           apxx_tcons1_inline.hh		\
 apxx_environment.hh         apxx_linexpr1_inline.hh    apxx_texpr0.hh			\
 apxx_environment_inline.hh  apxx_manager.hh            apxx_texpr0_inline.hh		\
 apxx_expr0.hh               apxx_manager_inline.hh     apxx_texpr1.hh			\
 apxx_expr1.hh               apxx_oct.hh                apxx_texpr1_inline.hh		\
 apxx_generator0.hh          apxx_oct_inline.hh         apxx_var.hh			\
 apxx_generator0_inline.hh   apxx_polka.hh              apxx_var_inline.hh


#---------------------------------------
# Rules
#---------------------------------------

all: libapronxx.a libapronxx_debug.a test

test: apronxx_test

clean:
	/bin/rm -f *.[ao] *.so apronxx_test
	/bin/rm -fr *~ \#*\# tmp

rebuild:

mostlyclean:

install:
	$(INSTALLd) $(INCDIR) $(INCDIR)/apronxx $(LIBDIR) $(BINDIR)
	$(INSTALL) $(CCINC) $(INCDIR)/apronxx
	$(INSTALL) libapronxx.a libapronxx_debug.a $(LIBDIR)

uninstall:
	/bin/rm -rf $(INCDIR)/apronxx
	/bin/rm -f $(LIBDIR)/linapronxx*.a


distclean: uninstall

dist: $(CCINC) $(CXXSOURCES) apronxx_test.cc apronxx_test_result.txt Makefile COPYING.GPL COPYING.LGPL README doc/Doxyfile
	(cd ..; tar zcvf apronxx.tgz $(^:%=apronxx/%))


#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .cc .hh .a .o .so

#-----------------------------------
# C++ part
#-----------------------------------

libapronxx.a: $(subst .cc,.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libapronxx_debug.a: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

apronxx_test: libapronxx_debug.a apronxx_test_debug.o
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ apronxx_test_debug.o $(LIBS)

%.o: %.cc $(CCINC)
	$(CXX) $(CXXFLAGS) $(ICXXFLAGS) $(DEFS) -c -o $@ $<

%_debug.o: %.cc $(CCINC)
	$(CXX) $(CXXFLAGS_DEBUG) $(ICXXFLAGS) $(DEFS) -c -o $@ $<


#-----------------------------------
# Documentation
#-----------------------------------

doc: $(CCINC) doc/Doxyfile
	cd doc && doxygen
	cd doc/latex && make

.PHONY: doc
