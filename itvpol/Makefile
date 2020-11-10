# Makefile
#
# APRON Library / Itvpol Domain
#
# Copyright (C) Liqian CHEN' 2009

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

ICFLAGS += $(BASE_ICFLAGS) $(ML_ICFLAGS) $(GLPK_ICFLAGS)
LDFLAGS += $(BASE_LIFLAGS) $(GLPK_ICFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CCSOURCES = ilp.c numitv.c itvp_user.c itvp_representation.c itvp_print.c itv_linear.c ifm.c itvp_user.c itvp_extract.c \
          itvp_domain.c itvp_transfer.c itvp_nary.c  itvp_predicate.c itvp_resize.c ibt.c 

CCINC = itvp_internal.h itvp_fun.h itvp_config.h numitv.h ilp.h itv_linear.h ifm.h itvp_user.h  itvp_domain.h ibt.h   
 

# trigers a whole recompilation
#DEPS = $(APRON_INCLUDE)/ap_abstract0.h

LIBS = -lapron -lmpfr -lgmp -lglpk -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lglpk -lm

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

all: D Dl 
#all: Il Ill MPZ Ri Rll MPQ D Dl MPFR
allml: allD allDl 
#allml: allIl allIll allMPZ allRi allRll allMPQ allD allDl allMPFR

allIl: Il mlIl
allIll: Ill mlIll
allMPZ: MPZ mlMPZ
allRi: Ri mlRi
allRll: Rll mlRll
allMPQ: MPQ mlMPQ
allD: D mlD
allDl: Dl mlDl
allMPFR: MPFR mlMPFR

Il: libitvpIl.a libitvpIl_debug.a itvptestIl
Ill: libitvpIll.a libitvpIll_debug.a itvptestIll
MPZ: libitvpMPZ.a libitvpMPZ_debug.a itvptestMPZ
Ri: libitvpRi.a libitvpRi_debug.a itvptestRi
Rll: libitvpRll.a libitvpRll_debug.a itvptestRll
MPQ: libitvpMPQ.a libitvpMPQ_debug.a itvptestMPQ
D: libitvpD.a libitvpD_debug.a itvptestD
Dl: libitvpDl.a libitvpDl_debug.a itvptestDl
MPFR: libitvpMPFR.a libitvpMPFR_debug.a itvptestMPFR
ifneq ($(HAS_SHARED),)
Il: libitvpIl.$(EXT_DLL) libitvpIl_debug.$(EXT_DLL)
Ill: libitvpIll.$(EXT_DLL) libitvpIll_debug.$(EXT_DLL)
MPZ: libitvpMPZ.$(EXT_DLL) libitvpMPZ_debug.$(EXT_DLL)
Ri: libitvpRi.$(EXT_DLL) libitvpRi_debug.$(EXT_DLL)
Rll: libitvpRll.$(EXT_DLL) libitvpRll_debug.$(EXT_DLL)
MPQ: libitvpMPQ.$(EXT_DLL) libitvpMPQ_debug.$(EXT_DLL)
D: libitvpD.$(EXT_DLL) libitvpD_debug.$(EXT_DLL)
Dl: libitvpDl.$(EXT_DLL) libitvpDl_debug.$(EXT_DLL)
MPFR: libitvpMPFR.$(EXT_DLL) libitvpMPFR_debug.$(EXT_DLL)
endif

mlexample%.byte: mlexample.ml itvp%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma itvp$*.cma $<

mlexample%.opt: mlexample.ml itvp%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa itvp$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) itvptest*
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr itvptop* itvprun* tmp
	/bin/rm -fr *~ \#*\#
	/bin/rm -fr itvp_caml.c itvp.ml itvp.mli
	/bin/rm -f Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_INCLUDE)/itvp $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) itvp.h $(APRON_INCLUDE)
	$(INSTALL) $(CCINC) $(APRON_INCLUDE)/itvp
	for i in itvptest* itvptop* itvprun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
	done
	for i in libitvp*.* libitvp*_debug.*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in libitvp*_caml.* dllitvp*_caml.$(EXT_DLL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in itvp.idl itvp.mli itvp.cmi itvp.cmx itvp*.cma itvp*.cmxa itvp*.a; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif

uninstall:
	/bin/rm -fr $(APRON_INCLUDE)/itvp.h
	/bin/rm -fr $(APRON_INCLUDE)/itvp
	/bin/rm -f $(APRON_BIN)/itvptest* $(APRON_BIN)/itvptop* $(APRON_BIN)/itvprun*
	/bin/rm -f $(APRON_LIB)/libitvp*.* $(APRON_LIB)/libitvp*_debug.*
	/bin/rm -f $(APRON_LIB)/dllitvp*.$(EXT_DLL) $(APRON_LIB)/dllitvp*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/itvp.mli $(APRON_LIB)/itvp.cm[ix] $(APRON_LIB)/itvp.idl $(APRON_LIB)/itvp*.cma $(APRON_LIB)/itvp*.cmx[as] $(APRON_LIB)/itvp*.a

dist: Makefile COPYING README itvp_doc.html perlscript_caml.pl perlscript_c.pl $(CCSOURCES) $(CCINC) itvp.h itvp_predicate.c itvp.idl itvp.mli itvp.ml itvp_caml.c
	(cd ..; tar zcvf itvpols.tgz $(^:%=itvpols/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o .$(EXT_DLL)

#-----------------------------------
# C part
#-----------------------------------

libitvp%.a: $(subst .c,%.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitvp%_debug.a: $(subst .c,%_debug.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitvp%.$(EXT_DLL): $(subst .c,%.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

libitvp%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)
		
itvptest%: libitvp%_debug.a itvp_example%.o
	$(CC) $(LDFLAGS) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ itvp_example$*.o \
	-L. -litvp$*_debug \
	-L../apron -lapron_debug \
	-L$(MPFR_PREFIX)/lib -lmpfr \
	-L$(GMP_PREFIX)/lib -lgmp \
	-L$(GLPK_PREFIX)/lib -lglpk \
	-lm

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

.PRECIOUS: libitvp%.a libitvp%_debug.a libitvp%.$(EXT_DLL) libitvp%_debug.$(EXT_DLL)
.PRECIOUS: libitvp%_caml.a libitvp%_caml_debug.a dllitvp%_caml.$(EXT_DLL)
.PRECIOUS: %Il.o %Ill.o %MPZ.o %Ri.o %Rll.o %MPQ.o %D.o %Dl.o %MPFR.o
.PRECIOUS: %Il_debug.o %Ill_debug.o %MPZ_debug.o %Ri_debug.o %Rll_debug.o %MPQ_debug.o %D_debug.o %Dl_debug.o %MPFR_debug.o

#-----------------------------------
# Caml part
#-----------------------------------

.INTERMEDIATE: mlIl

ml: itvp.mli itvp.cmi
mlIl: ml itvpIl.cma libitvpIl_caml.a libitvpIl_caml_debug.a
mlIll: ml itvpIll.cma libitvpIll_caml.a libitvpIll_caml_debug.a 
mlMPZ: ml itvpMPZ.cma libitvpMPZ_caml.a libitvpMPZ_caml_debug.a
mlRi: ml itvpRi.cma libitvpRi_caml.a libitvpRi_caml_debug.a
mlRll: ml itvpRll.cma libitvpRll_caml.a libitvpRll_caml_debug.a
mlMPQ: ml itvpMPQ.cma libitvpMPQ_caml.a libitvpMPQ_caml_debug.a 
mlD: ml itvpD.cma libitvpD_caml.a libitvpD_caml_debug.a
mlDl: ml itvpDl.cma libitvpDl_caml.a libitvpDl_caml_debug.a
mlMPFR: ml itvpMPFR.cma libitvpMPFR_caml.a libitvpMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlIl: $(call OCAMLOPT_TARGETS, itvpIl)
mlIll: $(call OCAMLOPT_TARGETS, itvpIll)
mlMPZ: $(call OCAMLOPT_TARGETS, itvpMPZ)
mlRi: $(call OCAMLOPT_TARGETS, itvpRi)
mlRll: $(call OCAMLOPT_TARGETS, itvpRll)
mlMPQ: $(call OCAMLOPT_TARGETS, itvpMPQ)
mlD: $(call OCAMLOPT_TARGETS, itvpD)
mlDl: $(call OCAMLOPT_TARGETS, itvpDl)
mlMPFR: $(call OCAMLOPT_TARGETS, itvpMPFR)
endif
ifneq ($(HAS_SHARED),)
mlIl: dllitvpIl_caml.$(EXT_DLL) 
mlIll:dllitvpIll_caml.$(EXT_DLL)
mlMPZ:dllitvpMPZ_caml.$(EXT_DLL) 
mlRll:dllitvpRll_caml.$(EXT_DLL)
mlMPQ:dllitvpMPQ_caml.$(EXT_DLL)
mlD:dllitvpD_caml.$(EXT_DLL)
mlDl:dllitvpDl_caml.$(EXT_DLL)
mlMPFR:dllitvpMPFR_caml.$(EXT_DLL)
endif

#itvptop%: itvp.cma libitvp_caml.a libitvp%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma itvp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-litvp_caml -litvp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp  -lglpk -lbigarray -lcamlidl"

#itvprun%: itvp.cma libitvp_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma itvp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-litvp_caml -litvp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lglpk -lbigarray -lcamlidl"

itvp%.cma: itvp.cmo libitvp%_caml.a libitvp%.a
	$(OCAMLMKLIB) -o itvp$* -oc itvp$*_caml itvp.cmo -litvp$* $(LIBS)

itvp%.cmxa itvp%.a: itvp.cmx libitvp%_caml.a libitvp%.a
	$(OCAMLMKLIB) -o itvp$* -oc itvp$*_caml itvp.cmx -litvp$* $(LIBS)

dllitvp%_caml.$(EXT_DLL) libitvp%_caml.a: itvp_caml.o libitvp%.a
	$(OCAMLMKLIB) -o itvp$*_caml $< -L. -litvp$* $(LDFLAGS) $(LIBS)

dllitvp%_caml_debug.$(EXT_DLL) libitvp%_caml_debug.a: itvp_caml_debug.o libitvp%_debug.a
	$(OCAMLMKLIB) -o itvp$*_caml_debug $< -L. -litvp$*_debug $(LDFLAGS) $(LIBS_DEBUG)

%.ml %.mli %_caml.c: %.idl perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp
	cd tmp && $(CAMLIDL) -no-include -nocpp -I . $*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

.PRECIOUS: %_caml.c %.ml %.mli %.cmi libitvp%_caml.a libitvp%_caml_debug.a dllitvp%_caml.$(EXT_DLL) itvp.cmx itvp.cmo

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
