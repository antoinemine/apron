# Makefile
#
# APRON Library / Octagonal Domain
#
# Copyright (C) Antoine Mine' 2006

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

CCSOURCES = oct_hmat.c oct_print.c oct_transfer.c oct_closure.c oct_nary.c \
	    oct_representation.c oct_predicate.c oct_resize.c

CCINC = oct_internal.h oct_fun.h

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

Il: liboctIl.a
Ill: liboctIll.a
MPZ: liboctMPZ.a
Ri: liboctRi.a
Rll: liboctRll.a
MPQ: liboctMPQ.a
D: liboctD.a
Dl: liboctDl.a
MPFR: liboctMPFR.a
ifneq ($(HAS_DEBUG),)
Il: liboctIl_debug.a octtestIl
Ill: liboctIll_debug.a octtestIll
MPZ: liboctMPZ_debug.a octtestMPZ
Ri: liboctRi_debug.a octtestRi
Rll: liboctRll_debug.a octtestRll
MPQ: liboctMPQ_debug.a octtestMPQ
D: liboctD_debug.a octtestD
Dl: liboctDl_debug.a octtestDl
MPFR: liboctMPFR_debug.a octtestMPFR
endif

ifneq ($(HAS_SHARED),)
Il: liboctIl.$(EXT_DLL)
Ill: liboctIll.$(EXT_DLL)
MPZ: liboctMPZ.$(EXT_DLL)
Ri: liboctRi.$(EXT_DLL)
Rll: liboctRll.$(EXT_DLL)
MPQ: liboctMPQ.$(EXT_DLL)
D: liboctD.$(EXT_DLL)
Dl: liboctDl.$(EXT_DLL)
MPFR: liboctMPFR.$(EXT_DLL)
ifneq ($(HAS_DEBUG),)
Il: liboctIl_debug.$(EXT_DLL)
Ill: liboctIll_debug.$(EXT_DLL)
MPZ: liboctMPZ_debug.$(EXT_DLL)
Ri: liboctRi_debug.$(EXT_DLL)
Rll: liboctRll_debug.$(EXT_DLL)
MPQ: liboctMPQ_debug.$(EXT_DLL)
D: liboctD_debug.$(EXT_DLL)
Dl: liboctDl_debug.$(EXT_DLL)
MPFR: liboctMPFR_debug.$(EXT_DLL)
endif
endif

mlexample%.byte: mlexample.ml oct%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma oct$*.cma $<

mlexample%.opt: mlexample.ml oct%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa oct$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) octtest*
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr octtop* octrun* tmp
	/bin/rm -fr *~ \#*\#
	/bin/rm -fr oct_caml.c oct.ml oct.mli
	/bin/rm -f Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_INCLUDE)/oct $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) -m 644 oct.h $(APRON_INCLUDE)
	$(INSTALL) -m 644 $(CCINC) $(APRON_INCLUDE)/oct
	for i in octtest* octtop* octrun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
	done
	for i in liboctIl.* liboctIll.* liboctMPZ.* liboctRi.* liboctRll.* liboctMPQ.* liboctD.* liboctDl.* liboctMPFR.* liboctIl_debug.* liboctIll_debug.* liboctMPZ_debug.* liboctRi_debug.* liboctRll_debug.* liboctMPQ_debug.* liboctD_debug.* liboctDl_debug.* liboctMPFR_debug.*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in liboct*_caml.*  liboct*_caml_debug.* dlloct*_caml.$(EXT_DLL) dlloct*_caml_debug.$(EXT_DLL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in oct.idl oct.mli oct.cmi oct.cmx oct*.cma oct*.cmxa oct*.a; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif

uninstall:
	/bin/rm -fr $(APRON_INCLUDE)/oct.h
	/bin/rm -fr $(APRON_INCLUDE)/oct
	/bin/rm -f $(APRON_BIN)/octtest* $(APRON_BIN)/octtop* $(APRON_BIN)/octrun*
	/bin/rm -f $(APRON_LIB)/liboct*.* $(APRON_LIB)/liboct*_debug.*
	/bin/rm -f $(APRON_LIB)/dlloct*.$(EXT_DLL) $(APRON_LIB)/dlloct*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/oct.mli $(APRON_LIB)/oct.cm[ix] $(APRON_LIB)/oct.idl $(APRON_LIB)/oct*.cma $(APRON_LIB)/oct*.cmx[as] $(APRON_LIB)/oct*.a

dist: Makefile COPYING README oct_doc.html perlscript_caml.pl perlscript_c.pl $(CCSOURCES) $(CCINC) oct.h oct_test.c oct.idl oct.mli oct.ml oct_caml.c
	(cd ..; tar zcvf octagons.tgz $(^:%=octagons/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o .$(EXT_DLL)

#-----------------------------------
# C part
#-----------------------------------

liboct%.a: $(subst .c,%.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

liboct%_debug.a: $(subst .c,%_debug.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

liboct%.$(EXT_DLL): $(subst .c,%.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

liboct%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

octtest%: oct_test%.o liboct%_debug.a
	$(CC) -o $@ oct_test$*.o \
		-L. -loct$*_debug -L../newpolka -lpolkaMPQ_debug \
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

.PRECIOUS: liboct%.a liboct%_debug.a liboct%.$(EXT_DLL) liboct%_debug.$(EXT_DLL)
.PRECIOUS: liboct%_caml.a liboct%_caml_debug.a dlloct%_caml.$(EXT_DLL)
.PRECIOUS: %Il.o %Ill.o %MPZ.o %Ri.o %Rll.o %MPQ.o %D.o %Dl.o %MPFR.o
.PRECIOUS: %Il_debug.o %Ill_debug.o %MPZ_debug.o %Ri_debug.o %Rll_debug.o %MPQ_debug.o %D_debug.o %Dl_debug.o %MPFR_debug.o

#-----------------------------------
# Caml part
#-----------------------------------

.INTERMEDIATE: mlIl

ml: oct.mli oct.cmi

mlIl: ml octIl.cma liboctIl_caml.a
mlIll: ml octIll.cma liboctIll_caml.a
mlMPZ: ml octMPZ.cma liboctMPZ_caml.a
mlRi: ml octRi.cma liboctRi_caml.a
mlRll: ml octRll.cma liboctRll_caml.a
mlMPQ: ml octMPQ.cma liboctMPQ_caml.a
mlD: ml octD.cma liboctD_caml.a
mlDl: ml octDl.cma liboctDl_caml.a
mlMPFR: ml octMPFR.cma liboctMPFR_caml.a

ifneq ($(HAS_OCAMLOPT),)
mlIl: $(call OCAMLOPT_TARGETS, octIl)
mlIll: $(call OCAMLOPT_TARGETS, octIll)
mlMPZ: $(call OCAMLOPT_TARGETS, octMPZ)
mlRi: $(call OCAMLOPT_TARGETS, octRi)
mlRll: $(call OCAMLOPT_TARGETS, octRll)
mlMPQ: $(call OCAMLOPT_TARGETS, octMPQ)
mlD: $(call OCAMLOPT_TARGETS, octD)
mlDl: $(call OCAMLOPT_TARGETS, octDl)
mlMPFR: $(call OCAMLOPT_TARGETS, octMPFR)
ifneq ($(HAS_DEBUG),)
mlIl: liboctIl_caml_debug.a $(call OCAMLOPT_TARGETS, octIl.d)
mlIll: liboctIll_caml_debug.a $(call OCAMLOPT_TARGETS, octIll.d)
mlMPZ: liboctMPZ_caml_debug.a $(call OCAMLOPT_TARGETS, octMPZ.d)
mlRi: liboctRi_caml_debug.a $(call OCAMLOPT_TARGETS, octRi.d)
mlRll: liboctRll_caml_debug.a $(call OCAMLOPT_TARGETS, octRll.d)
mlMPQ: liboctMPQ_caml_debug.a $(call OCAMLOPT_TARGETS, octMPQ.d)
mlD: liboctD_caml_debug.a $(call OCAMLOPT_TARGETS, octD.d)
mlDl: liboctDl_caml_debug.a $(call OCAMLOPT_TARGETS, octDl.d)
mlMPFR: liboctMPFR_caml_debug.a $(call OCAMLOPT_TARGETS, octMPFR.d)
endif
endif

#octtop%: oct.cma liboct_caml.a liboct%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma oct.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-loct_caml -loct$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

#octrun%: oct.cma liboct_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma oct.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-loct_caml -loct$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

oct%.cma: oct.cmo liboct%.a liboct%_caml.a
	$(OCAMLMKLIB) -o oct$* -oc oct$*_caml oct.cmo -loct$* $(LIBS)

oct%.cmxa oct%.a: oct.cmx liboct%.a liboct%_caml.a
	$(OCAMLMKLIB) -o oct$* -oc oct$*_caml oct.cmx -loct$* $(LIBS)

oct%.d.cmxa oct%.d.a: oct.cmx liboct%_debug.a liboct%_caml_debug.a
	$(OCAMLMKLIB) -o oct$*.d -oc oct$*_caml_debug oct.cmx -loct$*_debug $(LIBS_DEBUG)

dlloct%_caml.$(EXT_DLL) liboct%_caml.a: oct_caml.o liboct%.a
	$(OCAMLMKLIB) -o oct$*_caml $< -L. -loct$* $(LDFLAGS) $(LIBS)

dlloct%_caml_debug.$(EXT_DLL) liboct%_caml_debug.a: oct_caml_debug.o liboct%_debug.a
	$(OCAMLMKLIB) -o oct$*_caml_debug $< -L. -loct$*_debug $(LDFLAGS) $(LIBS_DEBUG)

%.ml %.mli %_caml.c: %.idl perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp
	cd tmp && $(CAMLIDL) -no-include -nocpp -I . $*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

.PRECIOUS: %_caml.c %.ml %.mli %.cmi liboct%_caml.a liboct%_caml_debug.a dlloct%_caml.$(EXT_DLL) oct.cmx oct.cmo

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
