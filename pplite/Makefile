# Makefile
#
# APRON Library / PPLite library wrapper
#
# Copyright (C) Antoine Mine' 2006
# Copyright (C) 2018-2023 Enea Zaffanella <enea.zaffanella@unipr.it>

# This file is part of the APRON Library, released under GPL license.
# Please read the COPYING file packaged in the distribution.

include ../Makefile.config
include ../vars.mk

#---------------------------------------
# Programs
#---------------------------------------

OCAMLMKLIB := $(OCAMLMKLIB) -ocamlc "$(OCAMLC) -cc $(CXX)"
OCAMLMKLIB := $(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT) -cc $(CXX)"
OCAMLMKLIB := $(OCAMLMKLIB) -dllpath $(PPLITE_PREFIX)/lib
OCAMLMKLIB := $(OCAMLMKLIB) -ccopt -L$(PPLITE_PREFIX)/lib

#---------------------------------------
# Flags
#---------------------------------------

ICFLAGS += $(BASE_ICFLAGS) $(ML_ICFLAGS)
ICXXFLAGS = $(ICFLAGS) $(PPLITE_ICFLAGS)
LDFLAGS += $(BASE_LIFLAGS) $(PPLITE_LIFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CXXSOURCES = pplite_user.cc pplite_poly.cc
CSOURCES = ap_pplite_test.c
CCINC = pplite_user.hh pplite_poly.hh ap_pplite.h

CCLIB_TO_INSTALL = libap_pplite.a
CCLIB_TO_INSTALL_DEBUG = libap_pplite_debug.a
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libap_pplite.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += libap_pplite_debug.$(EXT_DLL)
endif

CAMLLIB_TO_INSTALL = libap_pplite_caml.a pplite.idl pplite.cmi pplite.cma
CAMLLIB_TO_INSTALL_DEBUG = libap_pplite_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAMLLIB_TO_INSTALL += $(call OCAMLOPT_TARGETS, pplite)
CAMLLIB_TO_INSTALL_DEBUG = $(call OCAMLOPT_TARGETS, pplite.d)
endif
ifneq ($(HAS_SHARED),)
CAMLLIB_TO_INSTALL = dllap_pplite_caml.$(EXT_DLL)
CAMLLIB_TO_INSTALL_DEBUG = dllap_pplite_caml_debug.$(EXT_DLL)
endif

LIBS = -lapron -lpplite -lflint -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lpplite -lflint -lmpfr -lgmp -lm

#---------------------------------------
# Rules
#---------------------------------------

all: libap_pplite.a libap_pplite_debug.a ap_pplite_test ap_pplite_test_debug
ifneq ($(HAS_SHARED),)
all: libap_pplite.$(EXT_DLL) libap_pplite_debug.$(EXT_DLL)
endif

mlexample.byte: mlexample.ml pplite.cma
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -I ../apron -o $@ bigarray.cma gmp.cma apron.cma pplite.cma $<

mlexample.opt: mlexample.ml pplite.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -I ../apron -I . -o $@ bigarray.cmxa gmp.cmxa apron.cmxa pplite.cmxa $<

mlexample_adhoc.byte: mlexample_adhoc.ml pplite.cma
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -I ../apron -o $@ bigarray.cma gmp.cma apron.cma pplite.cma $<

mlexample_adhoc.opt: mlexample_adhoc.ml pplite.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -I ../apron -I . -o $@ bigarray.cmxa gmp.cmxa apron.cmxa pplite.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) ap_pplite_test ap_pplite_test_debug
	/bin/rm -f mlexample.byte mlexample.opt
	/bin/rm -f mlexample_adhoc.byte mlexample_adhoc.opt
	/bin/rm -f *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr *~ \#*\# tmp
	/bin/rm -f ap_pplite_caml.c pplite.ml pplite.mli

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) ap_pplite.h $(APRON_INCLUDE)
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
ifneq ($(HAS_DEBUG),)
	for i in $(CCLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
endif

uninstall:
	/bin/rm -f $(APRON_INCLUDE)/ap_pplite*
	/bin/rm -f $(APRON_LIB)/libap_pplite.* $(APRON_LIB)/libap_pplite_debug.*
ifeq ($(OCAMLFIND),)
	/bin/rm -f $(APRON_LIB)/libap_pplite_caml.* $(APRON_LIB)/dllap_pplite_caml.* $(APRON_LIB)/pplite.a $(APRON_LIB)/pplite.idl $(APRON_LIB)/pplite.cm*
	/bin/rm -f $(APRON_BIN)/*ap_pplite*
endif

dist: Makefile COPYING README $(CXXSOURCES) $(CSOURCES) $(CCINC) pplite.idl perlscript_c.pl perlscript_caml.pl ap_pplite_caml.c pplite.ml pplite.mli mlexample.ml mlexample_adhoc.ml
	(cd ..; tar zcvf pplite.tgz $(^:%=pplite/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .cc .hh .h .a .o .$(EXT_DLL)

#-----------------------------------
# C / C++ part
#-----------------------------------

libap_pplite.a: $(subst .cc,.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_pplite_debug.a: $(subst .cc,_debug.o,$(CXXSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_pplite.$(EXT_DLL): $(subst .cc,.o,$(CXXSOURCES))
	$(CXX_APRON_DYLIB) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
libap_pplite_debug.$(EXT_DLL): $(subst .cc,_debug.o,$(CXXSOURCES))
	$(CXX_APRON_DYLIB) $(CXXFLAGS_DEBUG) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

ifneq ($(HAS_SHARED),)
ap_pplite_test: libap_pplite.$(EXT_DLL)
ap_pplite_test_debug: libap_pplite_debug.$(EXT_DLL)
endif
ap_pplite_test: libap_pplite.a libap_pplite.$(EXT_DLL) ap_pplite_test.o
	$(CXX) $(CXXFLAGS) -o $@ ap_pplite_test.o \
		-L. -lap_pplite -L../newpolka -lpolkaMPQ \
		$(LDFLAGS) $(LIBS)
ap_pplite_test_debug: libap_pplite_debug.a libap_pplite_debug.$(EXT_DLL) ap_pplite_test_debug.o
	$(CXX) $(CXXFLAGS) -o $@ ap_pplite_test_debug.o \
		-L. -lap_pplite_debug -L../newpolka -lpolkaMPQ_debug \
		$(LDFLAGS) $(LIBS_DEBUG)

%.o: %.cc
	$(CXX) $(PPLITE_CXXFLAGS) $(ICXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

%_debug.o: %.cc
	$(CXX) $(PPLITE_CXXFLAGS_DEBUG) $(ICXXFLAGS) -c -o $@ $<

%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# Caml part
#-----------------------------------

ml: pplite.cma libap_pplite_caml.a libap_pplite_caml_debug.a

ifneq ($(HAS_OCAMLOPT),)
ml: $(call OCAMLOPT_TARGETS, pplite pplite.d)
endif

ifneq ($(HAS_SHARED),)
ml: dllap_pplite_caml.$(EXT_DLL)
endif

dllap_pplite_caml.$(EXT_DLL): libap_pplite_caml.a
libap_pplite_caml.a: ap_pplite_caml.o libap_pplite.a
	$(OCAMLMKLIB) -o ap_pplite_caml $< -L. -lap_pplite $(LDFLAGS) $(LIBS)

dllap_pplite_caml_debug.$(EXT_DLL): libap_pplite_caml_debug.a
libap_pplite_caml_debug.a: ap_pplite_caml_debug.o libap_pplite_debug.a
	$(OCAMLMKLIB) -o ap_pplite_caml_debug $< -L. -lap_pplite_debug $(LDFLAGS) $(LIBS_DEBUG)

pplite.cma: pplite.cmo libap_pplite_caml.a libap_pplite.a
	$(OCAMLMKLIB) -o pplite -oc ap_pplite_caml pplite.cmo -lap_pplite $(LIBS)

pplite.cmxa: pplite.a
pplite.a: pplite.cmx libap_pplite_caml.a libap_pplite.a
	$(OCAMLMKLIB) -o pplite -oc ap_pplite_caml pplite.cmx -lap_pplite $(LIBS)

pplite.d.cmxa: pplite.d.a
pplite.d.a: pplite.cmx libap_pplite_caml_debug.a libap_pplite_debug.a
	$(OCAMLMKLIB) -o pplite.d -oc ap_pplite_caml_debug pplite.cmx -lap_pplite_debug $(LIBS_DEBUG)


#---------------------------------------
# IDL rules
#---------------------------------------

%.ml %.mli ap_%_caml.c: %.idl perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp $*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c > ap_$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml > $*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli > $*.mli

rebuild:
	@echo "make rebuild is no longer necessary"


.PRECIOUS: %_caml.c %.ml %.mli %.cmi libap_pplite_caml.a dllap_pplite_caml.$(EXT_DLL) pplite.cmx pplite.cmo


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
