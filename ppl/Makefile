# Makefile
#
# APRON Library / PPL library wrapper
#
# Copyright (C) Antoine Mine' 2006

# This file is part of the APRON Library, released under GPL license.
# Please read the COPYING file packaged in the distribution.

include ../Makefile.config
include ../vars.mk

#---------------------------------------
# Programs
#---------------------------------------

OCAMLMKLIB := $(OCAMLMKLIB) -ocamlc "$(OCAMLC) -cc $(CXX)"
OCAMLMKLIB := $(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT) -cc $(CXX)"
OCAMLMKLIB := $(OCAMLMKLIB) -dllpath $(PPL_PREFIX)/lib

#---------------------------------------
# Flags
#---------------------------------------

ICFLAGS += $(BASE_ICFLAGS) $(ML_ICFLAGS) $(PPL_ICFLAGS)
ICXXFLAGS = $(ICFLAGS)
LDFLAGS += $(BASE_LIFLAGS) $(PPL_LIFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CXXSOURCES = ppl_user.cc ppl_poly.cc ppl_grid.cc
CSOURCES = ppl_test.c
CCINC = ppl_user.hh ppl_poly.hh ppl_grid.hh ppl_grid.h ap_ppl.h

LIBS = -lppl -lapron -lgmpxx -lmpfr -lgmp -lm
LIBS_DEBUG = -lppl -lapron_debug -lgmpxx -lmpfr -lgmp -lm

CCLIB_TO_INSTALL = libap_ppl.a
CCLIB_TO_INSTALL_DEBUG = libap_ppl_debug.a
CAMLLIB_TO_INSTALL = libap_ppl_caml.a
CAMLLIB_TO_INSTALL_DEBUG = libap_ppl_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAMLLIB_TO_INSTALL += $(call OCAMLOPT_TARGETS, ppl)
CAMLLIB_TO_INSTALL_DEBUG += $(call OCAMLOPT_TARGETS, ppl.d)
endif
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libap_ppl.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += libap_ppl_debug.$(EXT_DLL)
CAMLLIB_TO_INSTALL += dllap_ppl_caml.$(EXT_DLL)
CAMLLIB_TO_INSTALL_DEBUG += dllap_ppl_caml_debug.$(EXT_DLL)
endif


#---------------------------------------
# Rules
#---------------------------------------

all: libap_ppl.a libap_ppl_debug.a ap_ppl_test
ifneq ($(HAS_SHARED),)
all: libap_ppl.$(EXT_DLL) libap_ppl_debug.$(EXT_DLL)
endif

mlexample.byte: mlexample.ml ppl.cma
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -I ../apron -o $@ bigarray.cma gmp.cma apron.cma ppl.cma $<

mlexample.opt: mlexample.ml ppl.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -I ../apron -o $@ bigarray.cmxa gmp.cmxa apron.cmxa ppl.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) ap_ppl_test
	/bin/rm -f *.annot *.cm[ioax] *.cmx[as] pplrun ppltop
	/bin/rm -fr *~ \#*\# tmp
	/bin/rm -f ap_ppl_caml.c ppl.ml ppl.mli

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) ap_ppl.h $(APRON_INCLUDE)
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifneq ($(HAS_OCAML),)
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTALL) ppl.idl ppl.cmi ppl.cma; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in pplrun ppltop; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
	done
endif
endif
ifneq ($(HAS_DEBUG),)
	for i in $(CCLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifneq ($(HAS_OCAML),)
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
endif
endif

uninstall:
	/bin/rm -f $(APRON_INCLUDE)/ap_ppl*
	/bin/rm -f $(APRON_LIB)/libap_ppl.* $(APRON_LIB)/libap_ppl_debug.*
ifeq ($(OCAMLFIND),)
	/bin/rm -f $(APRON_LIB)/libap_ppl_caml.* $(APRON_LIB)/dllap_ppl_caml.* $(APRON_LIB)/ppl.a $(ARON_LIB)/ppl.idl $(APRON_LIB)/ppl.cm*
	/bin/rm -f $(APRON_BIN)/*ap_ppl*
	/bin/rm -f $(APRON_BIN)/pplrun $(APRON_BIN)/ppltop
endif

dist: Makefile COPYING README $(CXXSOURCES) $(CSOURCES) $(CCINC) ppl.idl perlscript_caml.pl ap_ppl_caml.c ppl.ml ppl.mli ppl.patch
	(cd ..; tar zcvf ppl.tgz $(^:%=ppl/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .cc .h .a .o .$(EXT_DLL)

#-----------------------------------
# C / C++ part
#-----------------------------------

libap_ppl.a: $(subst .cc,.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_ppl_debug.a: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_ppl.$(EXT_DLL): $(subst .cc,.o,$(CXXSOURCES))
	$(CXX_APRON_DYLIB) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
libap_ppl_debug.$(EXT_DLL): $(subst .cc,_debug.o,$(CXXSOURCES))
	$(CXX_APRON_DYLIB) $(CXXFLAGS_DEBUG) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

ifneq ($(HAS_SHARED),)
ap_ppl_test: libap_ppl_debug.$(EXT_DLL)
endif
ap_ppl_test: libap_ppl_debug.a libap_ppl_debug.$(EXT_DLL) ppl_test_debug.o
	$(CXX) $(CXXFLAGS) -o $@ ppl_test_debug.o \
		-L. -lap_ppl_debug -L../newpolka -lpolkaMPQ_debug \
		$(LDFLAGS) $(LIBS_DEBUG)

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
ml: $(call OCAMLOPT_TARGETS, ppl ppl.d)
endif

ifneq ($(HAS_SHARED),)
ml: dllap_ppl_caml.$(EXT_DLL)
endif

#ppltop: ppl.cma libap_ppl_caml.a libap_ppl.a
#	$(OCAMLMKTOP) $(OCAMLINC) $(OCAMLFLAGS) -o $@ -cc $(CXX) -custom \
#	bigarray.cma gmp.cma apron.cma ppl.cma \
#	-ccopt "-L. -L../apron  -L../itv -L$(MLGMPIDL_PREFIX)/lib -L../mlapronidl"

#pplrun: ppl.cma libap_ppl_caml.a
#	$(OCAMLC) $(OCAMLINC) $(OCAMLFLAGS) -o $@ -make-runtime -cc $(CXX) \
#	bigarray.cma gmp.cma apron.cma ppl.cma \
#	-ccopt "-L.  -L../apron -L../itv -L$(MLGMPIDL_PREFIX)/lib -L../mlapronidl"

dllap_ppl_caml.$(EXT_DLL): libap_ppl_caml.a
libap_ppl_caml.a: ap_ppl_caml.o libap_ppl.a
	$(OCAMLMKLIB) -o ap_ppl_caml $< -L. -lap_ppl $(LDFLAGS) $(LIBS)

dllap_ppl_caml_debug.$(EXT_DLL): libap_ppl_caml_debug.a
libap_ppl_caml_debug.a: ap_ppl_caml_debug.o libap_ppl_debug.a
	$(OCAMLMKLIB) -o ap_ppl_caml_debug $< -L. -lap_ppl_debug $(LDFLAGS) $(LIBS_DEBUG)

ppl.cma: ppl.cmo libap_ppl_caml.a libap_ppl.a
	$(OCAMLMKLIB) -o ppl -oc ap_ppl_caml ppl.cmo -lap_ppl $(LDFLAGS) $(LIBS)

ppl.cmxa: ppl.a
ppl.a: ppl.cmx libap_ppl_caml.a libap_ppl.a
	$(OCAMLMKLIB) -o ppl -oc ap_ppl_caml ppl.cmx -lap_ppl $(LDFLAGS) $(LIBS)

ppl.d.cmxa: ppl.d.a
ppl.d.a: ppl.cmx libap_ppl_caml_debug.a libap_ppl_debug.a
	$(OCAMLMKLIB) -o ppl.d -oc ap_ppl_caml_debug ppl.cmx -lap_ppl_debug $(LDFLAGS) $(LIBS_DEBUG)


#---------------------------------------
# IDL rules
#---------------------------------------

%.ml %.mli ap_%_caml.c: %.idl perlscript_caml.pl ../mlapronidl/manager.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/manager.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp $*.idl
	cp tmp/$*_stubs.c ap_$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"


.PRECIOUS: %_caml.c %.ml %.mli %.cmi libap_ppl_caml.a dllap_ppl_caml.$(EXT_DLL) ppl.cmx ppl.cmo


#---------------------------------------
# ML generic rules
#---------------------------------------

%.cmi: %.mli  $(DEPS)
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmo: %.ml %.cmi  $(DEPS)
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmx: %.ml %.cmi  $(DEPS)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -c $<

%.cmxs: %.cmxa
	$(OCAMLOPT_CMXS) $(CMXSINC) -o $@ $<
