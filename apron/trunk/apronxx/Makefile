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
# Flags
#---------------------------------------

# Machine-independent compilation flags.
ICXXFLAGS = \
-I../apron \
-I../num \
-I../itv \
-I../newpolka \
-I../octagons \
-I../ppl \
-I../products \
-I../box \
-I../taylor1plus \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

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
 apxx_generator0_inline.hh   apxx_polka.hh              apxx_var_inline.hh		\
 apxx_t1p.hh apxx_t1p_inline.hh	

LIBS = -L$(GMP_PREFIX)/lib -lm -lgmp -lgmpxx -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron

LIBS_DEBUG = -L$(GMP_PREFIX)/lib -lm -lgmp -lgmpxx -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron_debug

ifneq ($(HAS_PPL),)
DEFS := $(DEFS) -DHAS_PPL
LIBS := $(LIBS) -L$(PPL_PREFIX)/lib -L../ppl -L../products -lap_pkgrid -lap_ppl -lppl
LIBS_DEBUG := $(LIBS_DEBUG) -L$(PPL_PREFIX)/lib -L../ppl -L../products -lap_pkgrid_debug -lap_ppl_debug -lppl
ICXXFLAGS := $(ICXXFLAGS) -I$(PPL_PREFIX)/include  -I../ppl -I../products
endif

#---------------------------------------
# Rules
#---------------------------------------

all: libapronxx.a libapronxx_debug.a apronxx_test
ifneq ($(HAS_SHARED),)
all: libapronxx.so libapronxx_debug.so
endif

clean:
	/bin/rm -f *.[ao] *.so apronxx_test
	/bin/rm -fr *~ \#*\# tmp

distclean: clean

rebuild:


install:
	$(INSTALLd) $(INCDIR) $(INCDIR)/apronxx $(LIBDIR) $(BINDIR)
	$(INSTALL) $(CCINC) $(INCDIR)/apronxx
	$(INSTALL) libapronxx.* libapronxx_debug.* $(LIBDIR)

uninstall:
	/bin/rm -rf $(INCDIR)/apronxx
	/bin/rm -f $(LIBDIR)/libapronxx*.*


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

libapronxx.so: $(subst .cc,.o,$(CXXSOURCES))
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LIBS)

libapronxx_debug.so: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $^ $(LIBS_DEBUG)

apronxx_test: libapronxx_debug.a apronxx_test_debug.o
	$(CXX) $(CXXFLAGS_DEBUG) -o $@ apronxx_test_debug.o $(LIBS_DEBUG) -L../taylor1plus -lt1pMPQ_debug -L../newpolka -lpolkaMPQ_debug -L../octagons -loctMPQ_debug -L../box -lboxMPQ_debug -L. -lapronxx_debug

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
