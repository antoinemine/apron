# Makefile
#
# APRON Library / AV Octagonal Domain
#
# Copyright (C) Liqian Chen' 2014

# This file is part of the APRON Library, released under LGPL license
# with an exemption allowing the redistribution of statically linked
# executables.
#
# Please read the COPYING file packaged in the distribution.

include ../Makefile.config
include ../vars.mk

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

ICFLAGS += $(BASE_ICFLAGS) $(ML_ICFLAGS)
LDFLAGS += $(BASE_LIFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CCSOURCES = avo_hmat.c avo_print.c avo_transfer.c avo_closure.c avo_nary.c \
	    avo_representation.c avo_predicate.c avo_resize.c

CCINC = avo_internal.h avo_fun.h

# trigers a whole recompilation
#DEPS = $(APRON_INCLUDE)/ap_abstract0.h

LIBS = -lapron -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lm

#---------------------------------------
# Rules
#---------------------------------------

root:
	@echo
	@echo "Please choose a target from:"
	@echo
	@echo " Il Ill MPZ Rl Rll MPQ D Dl MPFR: single C library"
	@echo " all                     : all C libraries"
	@echo " ml                      : OCaml library"
	@echo " mlIl mlIll ...          : OCaml toplevel & byte-code"
	@echo " allml                   : everything"
	@echo " clean                   : remove objects"
	@echo " install                 : to install what has been compiled"
	@echo " uninstall               : clean & uninstall"
	@echo

all: Il Ill MPZ Ri Rll MPQ D Dl MPFR
allml: allIl allIll allMPZ allRi allRll allMPQ allD allDl allMPFR

allIl: Il mlIl
allIll: Ill mlIll
allMPZ: MPZ mlMPZ
allRi: Ri mlRi
allRll: Rll mlRll
allMPQ: MPQ mlMPQ
allD: D mlD
allDl: Dl mlDl
allMPFR: MPFR mlMPFR

Il: libavoIl.a libavoIl_debug.a avotestIl
Ill: libavoIll.a libavoIll_debug.a avotestIll
MPZ: libavoMPZ.a libavoMPZ_debug.a avotestMPZ
Ri: libavoRi.a libavoRi_debug.a avotestRi
Rll: libavoRll.a libavoRll_debug.a avotestRll
MPQ: libavoMPQ.a libavoMPQ_debug.a avotestMPQ
D: libavoD.a libavoD_debug.a avotestD
Dl: libavoDl.a libavoDl_debug.a avotestDl
MPFR: libavoMPFR.a libavoMPFR_debug.a avotestMPFR
ifneq ($(HAS_SHARED),)
Il: libavoIl.$(EXT_DLL) libavoIl_debug.$(EXT_DLL)
Ill: libavoIll.$(EXT_DLL) libavoIll_debug.$(EXT_DLL)
MPZ: libavoMPZ.$(EXT_DLL) libavoMPZ_debug.$(EXT_DLL)
Ri: libavoRi.$(EXT_DLL) libavoRi_debug.$(EXT_DLL)
Rll: libavoRll.$(EXT_DLL) libavoRll_debug.$(EXT_DLL)
MPQ: libavoMPQ.$(EXT_DLL) libavoMPQ_debug.$(EXT_DLL)
D: libavoD.$(EXT_DLL) libavoD_debug.$(EXT_DLL)
Dl: libavoDl.$(EXT_DLL) libavoDl_debug.$(EXT_DLL)
MPFR: libavoMPFR.$(EXT_DLL) libavoMPFR_debug.$(EXT_DLL)
endif

mlexample%.byte: mlexample.ml avo%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma avo$*.cma $<

mlexample%.opt: mlexample.ml avo%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa avo$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) avotest*
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr avotop* avorun* tmp
	/bin/rm -fr *~ \#*\#
	/bin/rm -fr avo_caml.c avo.ml avo.mli
	/bin/rm -f Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_INCLUDE)/avo $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) avo.h $(APRON_INCLUDE)
	$(INSTALL) $(CCINC) $(APRON_INCLUDE)/avo
	for i in avotest* avotop* avorun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
	done
	for i in libavo*.* libavo*_debug.*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in libavo*_caml.* dllavo*_caml.$(EXT_DLL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in avo.idl avo.mli avo.cmi avo.cmx avo*.cma avo*.cmxa avo*.a; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif

uninstall:
	/bin/rm -fr $(APRON_INCLUDE)/avo.h
	/bin/rm -fr $(APRON_INCLUDE)/avo
	/bin/rm -f $(APRON_BIN)/avotest* $(APRON_BIN)/avotop* $(APRON_BIN)/avorun*
	/bin/rm -f $(APRON_LIB)/libavo*.* $(APRON_LIB)/libavo*_debug.*
	/bin/rm -f $(APRON_LIB)/dllavo*.$(EXT_DLL) $(APRON_LIB)/dllavo*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/avo.mli $(APRON_LIB)/avo.cm[ix] $(APRON_LIB)/avo.idl $(APRON_LIB)/avo*.cma $(APRON_LIB)/avo*.cmx[as] $(APRON_LIB)/avo*.a

dist: Makefile COPYING README avo_doc.html perlscript_caml.pl perlscript_c.pl $(CCSOURCES) $(CCINC) avo.h avo_test.c avo.idl avo.mli avo.ml avo_caml.c
	(cd ..; tar zcvf avoct.tgz $(^:%=avoct/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o .$(EXT_DLL)

#-----------------------------------
# C part
#-----------------------------------

libavo%.a: $(subst .c,%.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libavo%_debug.a: $(subst .c,%_debug.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libavo%.$(EXT_DLL): $(subst .c,%.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

libavo%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

ifneq ($(HAS_SHARED),)
avotest%: avo_test%.o libavo%_debug.a libavo%_debug.$(EXT_DLL)
else
avotest%: avo_test%.o libavo%_debug.a
endif
	$(CC) -o $@ avo_test$*.o \
		-L. -lavo$*_debug \
		$(LDFLAGS) $(LIBS_DEBUG) $(CFLAGS_DEBUG)

%_caml.o: %_caml.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

%_caml_debug.o: %_caml.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

%Il.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGINT -c -o $@ $<
%Il_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGINT -c -o $@ $<
%Ill.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGINT -c -o $@ $<
%Ill_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGINT -c -o $@ $<
%MPZ.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPZ -c -o $@ $<
%MPZ_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPZ -c -o $@ $<

%Ri.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Ri_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rll.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%Rll_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%MPQ.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%MPQ_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<

%D.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%D_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%Dl.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<
%Dl_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<
%MPFR.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<
%MPFR_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<

# TODO: mpfr ?

.PRECIOUS: libavo%.a libavo%_debug.a libavo%.$(EXT_DLL) libavo%_debug.$(EXT_DLL)
.PRECIOUS: libavo%_caml.a libavo%_caml_debug.a dllavo%_caml.$(EXT_DLL)
.PRECIOUS: %Il.o %Ill.o %MPZ.o %Ri.o %Rll.o %MPQ.o %D.o %Dl.o %MPFR.o
.PRECIOUS: %Il_debug.o %Ill_debug.o %MPZ_debug.o %Ri_debug.o %Rll_debug.o %MPQ_debug.o %D_debug.o %Dl_debug.o %MPFR_debug.o

#-----------------------------------
# Caml part
#-----------------------------------

.INTERMEDIATE: mlIl

ml: avo.mli avo.cmi
mlIl: ml avoIl.cma libavoIl_caml.a libavoIl_caml_debug.a
mlIll: ml avoIll.cma libavoIll_caml.a libavoIll_caml_debug.a 
mlMPZ: ml avoMPZ.cma libavoMPZ_caml.a libavoMPZ_caml_debug.a
mlRi: ml avoRi.cma libavoRi_caml.a libavoRi_caml_debug.a
mlRll: ml avoRll.cma libavoRll_caml.a libavoRll_caml_debug.a
mlMPQ: ml avoMPQ.cma libavoMPQ_caml.a libavoMPQ_caml_debug.a 
mlD: ml avoD.cma libavoD_caml.a libavoD_caml_debug.a
mlDl: ml avoDl.cma libavoDl_caml.a libavoDl_caml_debug.a
mlMPFR: ml avoMPFR.cma libavoMPFR_caml.a libavoMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlIl: $(call OCAMLOPT_TARGETS, avoIl)
mlIll: $(call OCAMLOPT_TARGETS, avoIll)
mlMPZ: $(call OCAMLOPT_TARGETS, avoMPZ)
mlRi: $(call OCAMLOPT_TARGETS, avoRi)
mlRll: $(call OCAMLOPT_TARGETS, avoRll)
mlMPQ: $(call OCAMLOPT_TARGETS, avoMPQ)
mlD: $(call OCAMLOPT_TARGETS, avoD)
mlDl: $(call OCAMLOPT_TARGETS, avoDl)
mlMPFR: $(call OCAMLOPT_TARGETS, avoMPFR)
endif
ifneq ($(HAS_SHARED),)
mlIl: dllavoIl_caml.$(EXT_DLL) 
mlIll:dllavoIll_caml.$(EXT_DLL)
mlMPZ:dllavoMPZ_caml.$(EXT_DLL) 
mlRll:dllavoRll_caml.$(EXT_DLL)
mlMPQ:dllavoMPQ_caml.$(EXT_DLL)
mlD:dllavoD_caml.$(EXT_DLL)
mlDl:dllavoDl_caml.$(EXT_DLL)
mlMPFR:dllavoMPFR_caml.$(EXT_DLL)
endif

#avotop%: avo.cma libavo_caml.a libavo%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma avo.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lavo_caml -lavo$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

#avorun%: avo.cma libavo_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma avo.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lavo_caml -lavo$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

avo%.cma: avo.cmo libavo%_caml.a libavo%.a
	$(OCAMLMKLIB) -o avo$* -oc avo$*_caml avo.cmo -lavo$* $(LIBS)

avo%.cmxa avo%.a: avo.cmx libavo%_caml.a libavo%.a
	$(OCAMLMKLIB) -o avo$* -oc avo$*_caml avo.cmx -lavo$* $(LIBS)

dllavo%_caml.$(EXT_DLL) libavo%_caml.a: avo_caml.o libavo%.a
	$(OCAMLMKLIB) -o avo$*_caml $< -L. -lavo$* $(LDFLAGS) $(LIBS)

dllavo%_caml_debug.$(EXT_DLL) libavo%_caml_debug.a: avo_caml_debug.o libavo%_debug.a
	$(OCAMLMKLIB) -o avo$*_caml_debug $< -L. -lavo$*_debug $(LDFLAGS) $(LIBS_DEBUG)

%.ml %.mli %_caml.c: %.idl perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp
	cd tmp && $(CAMLIDL) -no-include -nocpp -I . $*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

.PRECIOUS: %_caml.c %.ml %.mli %.cmi libavo%_caml.a libavo%_caml_debug.a dllavo%_caml.$(EXT_DLL) avo.cmx avo.cmo

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

#-----------------------------------
# DEPENDENCIES
#-----------------------------------
