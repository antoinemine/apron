# Makefile
#
# APRON Library / PPL library wrapper
#
# Copyright (C) Antoine Mine' 2006

# This file is part of the APRON Library, released under GPL license.
# Please read the COPYING file packaged in the distribution.

include ../Makefile.config

PREFIX = $(APRON_PREFIX)

# C include and lib directories
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
BINDIR = $(PREFIX)/bin
CAMLDIR = $(PREFIX)/lib

SRCDIR = $(shell pwd)

ifneq ($(OCAMLFIND),)
MLGMPIDL_INCLUDE = $(shell $(OCAMLFIND) query gmp)
MLGMPIDL_LIB = $(MLGMPIDL_INCLUDE)
else
MLGMPIDL_INCLUDE = $(MLGMPIDL_PREFIX)/include
MLGMPIDL_LIB = $(MLGMPIDL_PREFIX)/lib
endif

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I$(MLGMPIDL_INCLUDE) \
-I../newpolka \
-I../apron \
-I../mlapronidl \
-I../num \
-I../itv \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(PPL_PREFIX)/include \
-I$(CAML_PREFIX) -I$(CAMLIDL_PREFIX)

ICXXFLAGS = \
-I../newpolka \
-I$(MLGMPIDL_INCLUDE) \
-I../apron \
-I../mlapronidl \
-I../num \
-I../itv \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(PPL_PREFIX)/include \
-I$(CAML_PREFIX) -I$(CAMLIDL_PREFIX)

# Caml
OCAMLINC = -I $(MLGMPIDL_LIB) -I ../mlapronidl

#---------------------------------------
# Files
#---------------------------------------

CXXSOURCES = ppl_user.cc ppl_poly.cc ppl_grid.cc
CSOURCES = ppl_test.c
CCINC = ppl_user.hh ppl_poly.hh ppl_grid.hh ppl_grid.h ap_ppl.h

LIBS = -L$(PPL_PREFIX)/lib -lppl -L$(APRON_PREFIX) -lapron -L$(GMP_PREFIX)/lib -lgmpxx -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -lstdc++ -lm
LIBS_DEBUG = -L$(PPL_PREFIX)/lib -lppl -L$(APRON_PREFIX) -lapron_debug -L$(GMP_PREFIX)/lib -lgmpxx -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -lstdc++ -lm

#---------------------------------------
# Rules
#---------------------------------------

all: libap_ppl.a libap_ppl_debug.a ap_ppl_test
ifneq ($(HAS_SHARED),)
all: libap_ppl.so libap_ppl_debug.so
endif

mlexample.byte: mlexample.ml ppl.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_PREFIX)/lib -o $@ bigarray.cma gmp.cma apron.cma ppl.cma $<

mlexample.opt: mlexample.ml ppl.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_PREFIX)/lib -o $@ bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa $<

clean:
	/bin/rm -f *.[ao] *.so ap_ppl_test
	/bin/rm -f *.annot *.cm[ioax] *.cmxa pplrun ppltop manager.idl
	/bin/rm -fr *~ \#*\# tmp
	/bin/rm -f ap_ppl_caml.c ppl.ml ppl.mli

distclean: clean

install:
	$(INSTALLd) $(INCDIR) $(LIBDIR) $(BINDIR) $(CAMLDIR)
	$(INSTALL) ap_ppl.h $(INCDIR)
	for i in libap_ppl.a libap_ppl_debug.a libap_ppl.so libap_ppl_debug.so; do \
		if test -f $$i; then $(INSTALL) $$i $(LIBDIR); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in libap_ppl_caml*.a dllap_ppl_caml*.so ppl.idl ppl.cmi ppl.cma ppl.cmxa ppl.a; do \
		if test -f $$i; then $(INSTALL) $$i $(CAMLDIR); fi; \
	done
	for i in ap_ppl_test pplrun ppltop; do \
		if test -f $$i; then $(INSTALL) $$i $(BINDIR); fi; \
	done
endif

uninstall:
	/bin/rm -f $(INCDIR)/ap_ppl*
	/bin/rm -f $(LIBDIR)/libap_ppl* $(LIBDIR)/dllap_ppl* $(LIBDIR)/ppl.a $(LIBDIR)/ppl.idl $(LIBDIR)/ppl.cm*
	/bin/rm -f $(BINDIR)/*ap_ppl*
	/bin/rm -f $(BINDIR)/pplrun $(BINDIR)/ppltop

dist: Makefile COPYING README $(CXXSOURCES) $(CSOURCES) $(CCINC) ppl.idl perlscript_caml.pl ap_ppl_caml.c ppl.ml ppl.mli ppl.patch
	(cd ..; tar zcvf ppl.tgz $(^:%=ppl/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .cc .h .a .o .so

#-----------------------------------
# C / C++ part
#-----------------------------------

libap_ppl.a: $(subst .cc,.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_ppl_debug.a: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_ppl.so: $(subst .cc,.o,$(CXXSOURCES))
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LIBS) -L../apron
libap_ppl_debug.so: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(CXX) $(CXXFLAGS_DEBUG) -shared -o $@ $^ $(LIBS_DEBUG) -L../apron

ap_ppl_test: libap_ppl_debug.a ppl_test_debug.o
	$(CXX) $(CXXFLAGS) -o $@ ppl_test_debug.o \
	-L. -lap_ppl_debug \
	-L../newpolka -L$(APRON_PREFIX)/lib -lpolkaMPQ_debug \
	-L../apron -L$(APRON_PREFIX)/lib -lapron_debug \
	-L$(PPL_PREFIX)/lib -lppl \
	-L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -lgmpxx -lmpfr -lgmp -lm

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(ICXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

%_debug.o: %.cc
	$(CXX) $(CXXFLAGS_DEBUG) $(ICXXFLAGS) -c -o $@ $<

%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# Caml part
#-----------------------------------

ml: ppl.cma libap_ppl_caml.a libap_ppl_caml_debug.a

ifneq ($(HAS_OCAMLOPT),)
ml: ppl.cmxa ppl.a
endif

ifneq ($(HAS_SHARED),)
ml: dllap_ppl_caml.so
endif

#ppltop: ppl.cma libap_ppl_caml.a libap_ppl.a
#	$(OCAMLMKTOP) $(OCAMLINC) $(OCAMLFLAGS) -o $@ -cc $(CXX) -custom \
#	bigarray.cma gmp.cma apron.cma ppl.cma \
#	-ccopt "-L. -L../apron  -L../itv -L$(MLGMPIDL_PREFIX)/lib -L../mlapronidl"

#pplrun: ppl.cma libap_ppl_caml.a
#	$(OCAMLC) $(OCAMLINC) $(OCAMLFLAGS) -o $@ -make-runtime -cc $(CXX) \
#	bigarray.cma gmp.cma apron.cma ppl.cma \
#	-ccopt "-L.  -L../apron -L../itv -L$(MLGMPIDL_PREFIX)/lib -L../mlapronidl"

libap_ppl_caml.a: ap_ppl_caml.o
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_ppl_caml_debug.a: ap_ppl_caml_debug.o
	$(AR) rcs $@ $^
	$(RANLIB) $@
dllap_ppl_caml.so: ap_ppl_caml.o libap_ppl.so
	$(CXX) $(CXXFLAGS) -shared -o $@ $<  -L. -lap_ppl $(LIBS) -L../apron

ppl.cma: ppl.cmo libap_ppl_caml.a
	$(OCAMLMKLIB) -ocamlc "$(OCAMLC) -cc $(CXX)" -verbose -o ppl -oc ap_ppl_caml ppl.cmo -L. -lap_ppl $(LIBS) -L../apron

ppl.cmxa ppl.a: ppl.cmx libap_ppl_caml.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT) -cc $(CXX)" -verbose -o ppl -oc ap_ppl_caml ppl.cmx -L. -lap_ppl $(LIBS) -L..:apron


#---------------------------------------
# IDL rules
#---------------------------------------

manager.idl: ../mlapronidl/manager.idl
	ln -s $< $@

ppl.ml ppl.mli ppl_caml.c: ppl.idl perlscript_caml.pl ../mlapronidl/manager.idl
	mkdir -p tmp
	cp ppl.idl ../mlapronidl/manager.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp ppl.idl
	cp tmp/ppl_stubs.c ap_ppl_caml.c
	$(PERL) perlscript_caml.pl < tmp/ppl.ml >ppl.ml
	$(PERL) perlscript_caml.pl < tmp/ppl.mli >ppl.mli

rebuild:
	@echo "make rebuild is no longer necessary"


.PRECIOUS: %_caml.c %.ml %.mli %.cmi libap_ppl_caml.a dllap_ppl_caml.so ppl.cmx ppl.cmo


#---------------------------------------
# ML generic rules
#---------------------------------------

%.cmi: %.mli  $(DEPS)
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmo: %.ml %.cmi  $(DEPS)
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmx: %.ml %.cmi  $(DEPS)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -c $<
