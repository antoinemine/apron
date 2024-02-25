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

CCLIB_TO_INSTALL = liboctIl.a liboctIll.a liboctMPZ.a liboctRl.a liboctRll.a  liboctMPQ.a liboctD.a  liboctDl.a  liboctMPFR.a
CCLIB_TO_INSTALL_DEBUG = liboctIl_debug.a liboctIll_debug.a liboctMPZ_debug.a liboctRl_debug.a liboctRll_debug.a  liboctMPQ_debug.a liboctD_debug.a  liboctDl_debug.a  liboctMPFR_debug.a
CAMLLIB_TO_INSTALL = liboctIl_caml.a liboctIll_caml.a liboctMPZ_caml.a liboctRl_caml.a liboctRll_caml.a liboctMPQ_caml.a liboctD_caml.a liboctDl_caml.a liboctMPFR_caml.a
CAMLLIB_TO_INSTALL_DEBUG = liboctIl_caml_debug.a liboctIll_caml_debug.a liboctMPZ_caml_debug.a liboctRl_caml_debug.a liboctRll_caml_debug.a  liboctMPQ_caml_debug.a liboctD_caml_debug.a liboctDl_caml_debug.a liboctMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAMLLIB_TO_INSTALL += $(call OCAMLOPT_TARGETS, $(addprefix oct, Il Ill MPZ Rl Rll MPQ D Dl MPFR))
CAMLLIB_TO_INSTALL_DEBUG += $(call OCAMLOPT_TARGETS, $(addprefix oct, Il.d Ill.d MPZ.d Rl.d Rll.d MPQ.d D.d Dl.d MPFR.d))
endif

ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += liboctIl.$(EXT_DLL) liboctIll.$(EXT_DLL) liboctMPZ.$(EXT_DLL) liboctRl.$(EXT_DLL) liboctRll.$(EXT_DLL)  liboctMPQ.$(EXT_DLL) liboctD.$(EXT_DLL)  liboctDl.$(EXT_DLL)  liboctMPFR.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += liboctIl_debug.$(EXT_DLL) liboctIll_debug.$(EXT_DLL) liboctMPZ_debug.$(EXT_DLL) liboctRl_debug.$(EXT_DLL) liboctRll_debug.$(EXT_DLL)  liboctMPQ_debug.$(EXT_DLL) liboctD_debug.$(EXT_DLL)  liboctDl_debug.$(EXT_DLL)  liboctMPFR_debug.$(EXT_DLL)
CAMLLIB_TO_INSTALL += dlloctIl_caml.$(EXT_DLL) dlloctIll_caml.$(EXT_DLL) dlloctMPZ_caml.$(EXT_DLL) dlloctRl_caml.$(EXT_DLL) dlloctRll_caml.$(EXT_DLL)  dlloctMPQ_caml.$(EXT_DLL) dlloctD_caml.$(EXT_DLL)  dlloctDl_caml.$(EXT_DLL)  dlloctMPFR_caml.$(EXT_DLL)
CAMLLIB_TO_INSTALL_DEBUG += dlloctIl_caml_debug.$(EXT_DLL) dlloctIll_caml_debug.$(EXT_DLL) dlloctMPZ_caml_debug.$(EXT_DLL) dlloctRl_caml_debug.$(EXT_DLL) dlloctRll_caml_debug.$(EXT_DLL)  dlloctMPQ_caml_debug.$(EXT_DLL) dlloctD_caml_debug.$(EXT_DLL)  dlloctDl_caml_debug.$(EXT_DLL)  dlloctMPFR_caml_debug.$(EXT_DLL)
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

Il: liboctIl.a liboctIl_debug.a octtestIl
Ill: liboctIll.a liboctIll_debug.a octtestIll
MPZ: liboctMPZ.a liboctMPZ_debug.a octtestMPZ
Ri: liboctRi.a liboctRi_debug.a octtestRi
Rll: liboctRll.a liboctRll_debug.a octtestRll
MPQ: liboctMPQ.a liboctMPQ_debug.a octtestMPQ
D: liboctD.a liboctD_debug.a octtestD
Dl: liboctDl.a liboctDl_debug.a octtestDl
MPFR: liboctMPFR.a liboctMPFR_debug.a octtestMPFR
ifneq ($(HAS_SHARED),)
Il: liboctIl.$(EXT_DLL) liboctIl_debug.$(EXT_DLL)
Ill: liboctIll.$(EXT_DLL) liboctIll_debug.$(EXT_DLL)
MPZ: liboctMPZ.$(EXT_DLL) liboctMPZ_debug.$(EXT_DLL)
Ri: liboctRi.$(EXT_DLL) liboctRi_debug.$(EXT_DLL)
Rll: liboctRll.$(EXT_DLL) liboctRll_debug.$(EXT_DLL)
MPQ: liboctMPQ.$(EXT_DLL) liboctMPQ_debug.$(EXT_DLL)
D: liboctD.$(EXT_DLL) liboctD_debug.$(EXT_DLL)
Dl: liboctDl.$(EXT_DLL) liboctDl_debug.$(EXT_DLL)
MPFR: liboctMPFR.$(EXT_DLL) liboctMPFR_debug.$(EXT_DLL)
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
	$(INSTALL) oct.h $(APRON_INCLUDE)
	$(INSTALL) $(CCINC) $(APRON_INCLUDE)/oct
	for i in octtop* octrun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); $(STRIP) $(APRON_BIN)/$$i; fi; \
	done
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTAL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in oct.idl oct.mli oct.cmi oct.cmx oct*.cma oct*.cmxa oct*.a; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
ifneq ($(HAS_DEBUG),)
	for i in $(CCLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAMLLIB_TO_INSTAL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
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

ifneq ($(HAS_SHARED),)
octtest%: oct_test%.o liboct%_debug.a liboct%_debug.$(EXT_DLL)
else
octtest%: oct_test%.o liboct%_debug.a
endif
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
mlIl: ml octIl.cma liboctIl_caml.a liboctIl_caml_debug.a
mlIll: ml octIll.cma liboctIll_caml.a liboctIll_caml_debug.a 
mlMPZ: ml octMPZ.cma liboctMPZ_caml.a liboctMPZ_caml_debug.a
mlRi: ml octRi.cma liboctRi_caml.a liboctRi_caml_debug.a
mlRll: ml octRll.cma liboctRll_caml.a liboctRll_caml_debug.a
mlMPQ: ml octMPQ.cma liboctMPQ_caml.a liboctMPQ_caml_debug.a 
mlD: ml octD.cma liboctD_caml.a liboctD_caml_debug.a
mlDl: ml octDl.cma liboctDl_caml.a liboctDl_caml_debug.a
mlMPFR: ml octMPFR.cma liboctMPFR_caml.a liboctMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlIl: $(call OCAMLOPT_TARGETS, octIl octIl.d)
mlIll: $(call OCAMLOPT_TARGETS, octIll octIll.d)
mlMPZ: $(call OCAMLOPT_TARGETS, octMPZ octMPZ.d)
mlRi: $(call OCAMLOPT_TARGETS, octRi octRi.d)
mlRll: $(call OCAMLOPT_TARGETS, octRll octRll.d)
mlMPQ: $(call OCAMLOPT_TARGETS, octMPQ octMPQ.d)
mlD: $(call OCAMLOPT_TARGETS, octD octD.d)
mlDl: $(call OCAMLOPT_TARGETS, octDl octDl.d)
mlMPFR: $(call OCAMLOPT_TARGETS, octMPFR octMPFR.d)
endif
ifneq ($(HAS_SHARED),)
mlIl: dlloctIl_caml.$(EXT_DLL) 
mlIll:dlloctIll_caml.$(EXT_DLL)
mlMPZ:dlloctMPZ_caml.$(EXT_DLL) 
mlRll:dlloctRll_caml.$(EXT_DLL)
mlMPQ:dlloctMPQ_caml.$(EXT_DLL)
mlD:dlloctD_caml.$(EXT_DLL)
mlDl:dlloctDl_caml.$(EXT_DLL)
mlMPFR:dlloctMPFR_caml.$(EXT_DLL)
endif

#octtop%: oct.cma liboct_caml.a liboct%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma oct.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-loct_caml -loct$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

#octrun%: oct.cma liboct_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma oct.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-loct_caml -loct$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lbigarray -lcamlidl"

oct%.cma: oct.cmo liboct%_caml.a liboct%.a
	$(OCAMLMKLIB) -o oct$* -oc oct$*_caml oct.cmo -loct$* $(LIBS)

oct%.cmxa oct%.a: oct.cmx liboct%_caml.a liboct%.a
	$(OCAMLMKLIB) -o oct$* -oc oct$*_caml oct.cmx -loct$* $(LIBS)

oct%.d.cmxa oct%.d.a: oct.cmx liboct%_caml_debug.a liboct%_debug.a
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
