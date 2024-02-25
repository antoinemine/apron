# Makefile
#
# APRON Library / Fppol Domain
#
# Copyright (C) Liqian CHEN' 2008

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
LDFLAGS += $(BASE_LIFLAGS) $(GLPK_LIFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CCSOURCES = fpp_print.c fpp_transfer.c fpp_user.c fpp_extract.c fpp_nary.c \
	    fpp_representation.c fpp_predicate.c fpp_resize.c fpp_weakjoin.c fppol_domain.c bt.c quasi_linear.c rlp.c fm.c  numitv.c bv.c

CCINC = fpp_internal.h fpp_fun.h fpp_weakjoin.h fppol_domain.h rlp.h bt.h quasi_linear.h fm.h numitv.h bv.h

# trigers a whole recompilation
#DEPS = $(APRON_INCLUDE)/ap_abstract0.h

LIBS = -lapron -lmpfr -lgmp -lglpk -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lglpk -lm

CCLIB_TO_INSTALL = libfppD.a  libfppDl.a
CCLIB_TO_INSTALL_DEBUG = libfppD_debug.a libfppDl_debug.a
CAMLLIB_TO_INSTALL = libfppD_caml.a libfppDl_caml.a
CAMLLIB_TO_INSTALL_DEBUG = libfppD_caml_debug.a libfppDl_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAMLLIB_TO_INSTALL += $(call OCAMLOPT_TARGETS, fppD fppDl)
CAMLLIB_TO_INSTALL_DEBUG = $(call OCAMLOPT_TARGETS, fppD.d fppDl.d)
endif
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libfppD.$(EXT_DLL) libfppDl.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += libfppD_debug.$(EXT_DLL) libfppDl_debug.$(EXT_DLL)
CAMLLIB_TO_INSTALL += dllfppD_caml.$(EXT_DLL) dllfppDl_caml.$(EXT_DLL)
CAMLLIB_TO_INSTALL_DEBUG += dllfppD_caml_debug.$(EXT_DLL) dllfppDl_caml_debug.$(EXT_DLL)
endif


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

Il: libfppIl.a libfppIl_debug.a fpptestIl
Ill: libfppIll.a libfppIll_debug.a fpptestIll
MPZ: libfppMPZ.a libfppMPZ_debug.a fpptestMPZ
Ri: libfppRi.a libfppRi_debug.a fpptestRi
Rll: libfppRll.a libfppRll_debug.a fpptestRll
MPQ: libfppMPQ.a libfppMPQ_debug.a fpptestMPQ
D: libfppD.a libfppD_debug.a fpptestD
Dl: libfppDl.a libfppDl_debug.a fpptestDl
MPFR: libfppMPFR.a libfppMPFR_debug.a fpptestMPFR
ifneq ($(HAS_SHARED),)
Il: libfppIl.$(EXT_DLL) libfppIl_debug.$(EXT_DLL)
Ill: libfppIll.$(EXT_DLL) libfppIll_debug.$(EXT_DLL)
MPZ: libfppMPZ.$(EXT_DLL) libfppMPZ_debug.$(EXT_DLL)
Ri: libfppRi.$(EXT_DLL) libfppRi_debug.$(EXT_DLL)
Rll: libfppRll.$(EXT_DLL) libfppRll_debug.$(EXT_DLL)
MPQ: libfppMPQ.$(EXT_DLL) libfppMPQ_debug.$(EXT_DLL)
D: libfppD.$(EXT_DLL) libfppD_debug.$(EXT_DLL)
Dl: libfppDl.$(EXT_DLL) libfppDl_debug.$(EXT_DLL)
MPFR: libfppMPFR.$(EXT_DLL) libfppMPFR_debug.$(EXT_DLL)
endif

mlexample%.byte: mlexample.ml fpp%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma fpp$*.cma $<

mlexample%.opt: mlexample.ml fpp%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa fpp$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) fpptest*
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr fpptop* fpprun* tmp
	/bin/rm -fr *~ \#*\#
	/bin/rm -fr fpp_caml.c fpp.ml fpp.mli
	/bin/rm -f Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_INCLUDE)/fpp $(APRON_LIB) $(APRON_BIN)
	$(INSTALL) fpp.h $(APRON_INCLUDE)
	$(INSTALL) $(CCINC) $(APRON_INCLUDE)/fpp
	for i in fpptop* fpprun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); $(STRIP) $(APRON_BIN)/$$i; fi; \
	done
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in fpp.idl fpp.mli fpp.cmi fpp.cmx fpp*.cma fpp*.cmxa fpp*.a; do \
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
	/bin/rm -fr $(APRON_INCLUDE)/fpp.h
	/bin/rm -fr $(APRON_INCLUDE)/fpp
	/bin/rm -f $(APRON_BIN)/fpptest* $(APRON_BIN)/fpptop* $(APRON_BIN)/fpprun*
	/bin/rm -f $(APRON_LIB)/libfpp*.* $(APRON_LIB)/libfpp*_debug.*
	/bin/rm -f $(APRON_LIB)/dllfpp*.$(EXT_DLL) $(APRON_LIB)/dllfpp*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/fpp.mli $(APRON_LIB)/fpp.cm[ix] $(APRON_LIB)/fpp.idl $(APRON_LIB)/fpp*.cma $(APRON_LIB)/fpp*.cmx[as] $(APRON_LIB)/fpp*.a

dist: Makefile COPYING README fpp_doc.html perlscript_caml.pl perlscript_c.pl $(CCSOURCES) $(CCINC) fpp.h fpp_predicate.c fpp.idl fpp.mli fpp.ml fpp_caml.c
	(cd ..; tar zcvf fppols.tgz $(^:%=fppols/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o .$(EXT_DLL)

#-----------------------------------
# C part
#-----------------------------------

libfpp%.a: $(subst .c,%.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libfpp%_debug.a: $(subst .c,%_debug.o,$(CCSOURCES))
	$(AR) rcs $@ $^
	$(RANLIB) $@

libfpp%.$(EXT_DLL): $(subst .c,%.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

libfpp%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCSOURCES))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

ifneq ($(HAS_SHARED),)
fpptest%: fpp_example%.o libfpp%_debug.a libfpp%_debug.$(EXT_DLL)
else
fpptest%: fpp_example%.o libfpp%_debug.a
endif
	$(CC) $(LDFLAGS) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ fpp_example$*.o \
	-L. -lfpp$*_debug \
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

.PRECIOUS: libfpp%.a libfpp%_debug.a libfpp%.$(EXT_DLL) libfpp%_debug.$(EXT_DLL)
.PRECIOUS: libfpp%_caml.a libfpp%_caml_debug.a dllfpp%_caml.$(EXT_DLL)
.PRECIOUS: %Il.o %Ill.o %MPZ.o %Ri.o %Rll.o %MPQ.o %D.o %Dl.o %MPFR.o
.PRECIOUS: %Il_debug.o %Ill_debug.o %MPZ_debug.o %Ri_debug.o %Rll_debug.o %MPQ_debug.o %D_debug.o %Dl_debug.o %MPFR_debug.o

#-----------------------------------
# Caml part
#-----------------------------------

.INTERMEDIATE: mlIl

ml: fpp.mli fpp.cmi
mlIl: ml fppIl.cma libfppIl_caml.a libfppIl_caml_debug.a
mlIll: ml fppIll.cma libfppIll_caml.a libfppIll_caml_debug.a 
mlMPZ: ml fppMPZ.cma libfppMPZ_caml.a libfppMPZ_caml_debug.a
mlRi: ml fppRi.cma libfppRi_caml.a libfppRi_caml_debug.a
mlRll: ml fppRll.cma libfppRll_caml.a libfppRll_caml_debug.a
mlMPQ: ml fppMPQ.cma libfppMPQ_caml.a libfppMPQ_caml_debug.a 
mlD: ml fppD.cma libfppD_caml.a libfppD_caml_debug.a
mlDl: ml fppDl.cma libfppDl_caml.a libfppDl_caml_debug.a
mlMPFR: ml fppMPFR.cma libfppMPFR_caml.a libfppMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlIl: $(call OCAMLOPT_TARGETS, fppIl fppIl.d)
mlIll: $(call OCAMLOPT_TARGETS, fppIll fppIll.d)
mlMPZ: $(call OCAMLOPT_TARGETS, fppMPZ fppMPZ.d)
mlRi: $(call OCAMLOPT_TARGETS, fppRi fppRi.d)
mlRll: $(call OCAMLOPT_TARGETS, fppRll fppRll.d)
mlMPQ: $(call OCAMLOPT_TARGETS, fppMPQ fppMPQ.d)
mlD: $(call OCAMLOPT_TARGETS, fppD fppD.d)
mlDl: $(call OCAMLOPT_TARGETS, fppDl fppDl.d)
mlMPFR: $(call OCAMLOPT_TARGETS, fppMPFR fppMPFR.d)
endif
ifneq ($(HAS_SHARED),)
mlIl: dllfppIl_caml.$(EXT_DLL) 
mlIll:dllfppIll_caml.$(EXT_DLL)
mlMPZ:dllfppMPZ_caml.$(EXT_DLL) 
mlRll:dllfppRll_caml.$(EXT_DLL)
mlMPQ:dllfppMPQ_caml.$(EXT_DLL)
mlD:dllfppD_caml.$(EXT_DLL)
mlDl:dllfppDl_caml.$(EXT_DLL)
mlMPFR:dllfppMPFR_caml.$(EXT_DLL)
endif

#fpptop%: fpp.cma libfpp_caml.a libfpp%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma fpp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lfpp_caml -lfpp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp  -lglpk -lbigarray -lcamlidl"

#fpprun%: fpp.cma libfpp_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma fpp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lfpp_caml -lfpp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lglpk -lbigarray -lcamlidl"

fpp%.cma: fpp.cmo libfpp%_caml.a libfpp%.a
	$(OCAMLMKLIB) -o fpp$* -oc fpp$*_caml fpp.cmo -lfpp$* $(LDFLAGS) $(LIBS)

fpp%.cmxa fpp%.a: fpp.cmx libfpp%_caml.a libfpp%.a
	$(OCAMLMKLIB) -o fpp$* -oc fpp$*_caml fpp.cmx -lfpp$* $(LDFLAGS) $(LIBS)

fpp%.d.cmxa fpp%.d.a: fpp.cmx libfpp%_caml_debug.a libfpp%_debug.a
	$(OCAMLMKLIB) -o fpp$*.d -oc fpp$*_caml_debug fpp.cmx -lfpp$*_debug $(LDFLAGS) $(LIBS_DEBUG)

dllfpp%_caml.$(EXT_DLL) libfpp%_caml.a: fpp_caml.o libfpp%.a
	$(OCAMLMKLIB) -o fpp$*_caml $< -L. -lfpp$* $(LDFLAGS) $(LIBS)

dllfpp%_caml_debug.$(EXT_DLL) libfpp%_caml_debug.a: fpp_caml_debug.o libfpp%_debug.a
	$(OCAMLMKLIB) -o fpp$*_caml_debug $< -L. -lfpp$*_debug $(LDFLAGS) $(LIBS_DEBUG)

%.ml %.mli %_caml.c: %.idl perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp
	cd tmp && $(CAMLIDL) -no-include -nocpp -I . $*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

.PRECIOUS: %_caml.c %.ml %.mli %.cmi libfpp%_caml.a libfpp%_caml_debug.a dllfpp%_caml.$(EXT_DLL) fpp.cmx fpp.cmo

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
