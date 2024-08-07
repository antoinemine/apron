# Makefile
#
# APRON Library / C++ class wrappers
#
# Copyright (C) Antoine Mine' 2007

# This file is part of the APRON Library, released under LGPL license
# with an exception allowing the redistribution of statically linked
# executables.

# Please read the COPYING file packaged in the distribution.



include ../Makefile.config
include ../vars.mk

#---------------------------------------
# Flags
#---------------------------------------

# Machine-independent compilation flags.
ICXXFLAGS = $(ICFLAGS) $(BASE_ICFLAGS) \
	-I../box -I../newpolka -I../octagons -I../taylor1plus
LDFLAGS += $(BASE_LIFLAGS)

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

LIBS = $(LDFLAGS) -lapron  -lmpfr -lgmpxx -lgmp -lm
LIBS_DEBUG = $(LDFLAGS) -lapron_debug -lmpfr -lgmpxx -lgmp -lm

CCLIB_TO_INSTALL = libapronxx.a
CCLIB_TO_INSTALL_DEBUG = libapronxx_debug.a

ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libapronxx.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += libapronxx_debug.$(EXT_DLL)
endif

ifneq ($(HAS_PPL),)
  DEFS := $(DEFS) -DHAS_PPL
  LIBS := $(LIBS) $(PPL_LIFLAGS) -L../ppl -L../products -lap_pkgrid -lap_ppl -lppl
  LIBS_DEBUG := $(LIBS_DEBUG) $(PPL_LIFLAGS) -L../ppl -L../products -lap_pkgrid_debug -lap_ppl_debug -lppl
  ICXXFLAGS := $(ICXXFLAGS) $(PPL_ICFLAGS) -I../ppl -I../products
endif

#---------------------------------------
# Rules
#---------------------------------------

all: libapronxx.a libapronxx_debug.a apronxx_test
ifneq ($(HAS_SHARED),)
all: libapronxx.$(EXT_DLL) libapronxx_debug.$(EXT_DLL)
endif

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) apronxx_test
	/bin/rm -fr *~ \#*\# tmp

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_INCLUDE)/apronxx $(APRON_LIB)
	$(INSTALL) $(CCINC) $(APRON_INCLUDE)/apronxx
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifneq ($(HAS_DEBUG),)
	$(INSTALL) $(CCLIB_TO_INSTALL_DEBUG) $(APRON_LIB)
endif

uninstall:
	/bin/rm -rf $(APRON_INCLUDE)/apronxx
	/bin/rm -f $(APRON_LIB)/libapronxx*.*


dist: $(CCINC) $(CXXSOURCES) apronxx_test.cc apronxx_test_result.txt Makefile COPYING.GPL COPYING.LGPL README doc/Doxyfile
	(cd ..; tar zcvf apronxx.tgz $(^:%=apronxx/%))


#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .cc .hh .a .o .$(EXT_DLL)

#-----------------------------------
# C++ part
#-----------------------------------

libapronxx.a: $(subst .cc,.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libapronxx_debug.a: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libapronxx.$(EXT_DLL): $(subst .cc,.o,$(CXXSOURCES))
	$(CXX) $(CFLAGS) -shared -o $@ $^ $(LIBS)

libapronxx_debug.$(EXT_DLL): $(subst .cc,_debug.o,$(CXXSOURCES))
	$(CXX) $(CFLAGS_DEBUG) -shared -o $@ $^ $(LIBS_DEBUG)

apronxx_test: libapronxx_debug.a apronxx_test_debug.o
	$(CXX) $(CFLAGS_DEBUG) -o $@ apronxx_test_debug.o -L../newpolka -lpolkaMPQ_debug -L../octagons -loctMPQ_debug -L../box -lboxMPQ_debug -L../taylor1plus -lt1pMPQ_debug libapronxx_debug.a $(LIBS_DEBUG)

%.o: %.cc $(CCINC)
	$(CXX) $(CXXFLAGS) $(ICXXFLAGS) $(DEFS) -c -o $@ $<

%_debug.o: %.cc $(CCINC)
	$(CXX) $(CXXFLAGS_DEBUG) $(ICXXFLAGS) $(DEFS) -c -o $@ $<


#-----------------------------------
# Documentation
#-----------------------------------

doc: $(CCINC) doc/Doxyfile
	cd doc && doxygen

.PHONY: doc
