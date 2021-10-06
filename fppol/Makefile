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
LDFLAGS += $(BASE_LIFLAGS) $(GLPK_ICFLAGS)
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

#---------------------------------------
# Rules
#---------------------------------------

root:
	@echo
	@echo "Please choose a target from:"
	@echo
	@echo " D Dl: single C library"
	@echo " all                     : all C libraries"
	@echo " ml                      : OCaml library"
	@echo " mlD mlD     ..          : OCaml toplevel & byte-code"
	@echo " allml                   : everything"
	@echo " clean                   : remove objects"
	@echo " install                 : to install what has been compiled"
	@echo " uninstall               : clean & uninstall"
	@echo

all: D Dl 
allml: allD allDl 

allD: D mlD
allDl: Dl mlDl

D: libfppD.a
Dl: libfppDl.a
ifneq ($(HAS_DEBUG),)
D: libfppD_debug.a fpptestD
Dl: libfppDl_debug.a fpptestDl
endif

ifneq ($(HAS_SHARED),)
D: libfppD.$(EXT_DLL)
Dl: libfppDl.$(EXT_DLL)
ifneq ($(HAS_DEBUG),)
D: libfppD_debug.$(EXT_DLL)
Dl: libfppDl_debug.$(EXT_DLL)
endif
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
	$(INSTALL) -m 644 fpp.h $(APRON_INCLUDE)
	$(INSTALL) -m 644 $(CCINC) $(APRON_INCLUDE)/fpp
	for i in fpptest* fpptop* fpprun*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
	done
	for i in libfppD.* libfppDl.* libfppD_debug.* libfppDl_debug.*; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in libfpp*_caml.* dllfpp*_caml.$(EXT_DLL) libfpp*_caml_debug.* dllfpp*_caml_debug.$(EXT_DLL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
	for i in fpp.idl fpp.mli fpp.cmi fpp.cmx fpp*.cma fpp*.cmxa fpp*.a fpp*.d.cmxa fpp*.d.a; do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
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

%D.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%D_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%Dl.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<
%Dl_debug.o: %.c $(CCINC) $(DEPS)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<

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

mlD: ml fppD.cma libfppD_caml.a
mlDl: ml fppDl.cma libfppDl_caml.a

ifneq ($(HAS_OCAMLOPT),)
mlD: $(call OCAMLOPT_TARGETS, fppD)
mlDl: $(call OCAMLOPT_TARGETS, fppDl)
ifneq ($(HAS_DEBUG),)
mlD: libfppD_caml_debug.a $(call OCAMLOPT_TARGETS, fppD.d)
mlDl: libfppDl_caml_debug.a $(call OCAMLOPT_TARGETS, fppDl.d)
endif
endif


#fpptop%: fpp.cma libfpp_caml.a libfpp%.a
#	$(OCAMLMKTOP) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -custom bigarray.cma gmp.cma apron.cma fpp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lfpp_caml -lfpp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp  -lglpk -lbigarray -lcamlidl"

#fpprun%: fpp.cma libfpp_caml.a
#	$(OCAMLC) -noautolink $(OCAMLFLAGS) $(OCAMLINC) -o $@ -make-runtime bigarray.cma gmp.cma apron.cma fpp.cma -ccopt "-L. -L../mlapronidl $(LDFLAGS)" -cclib "-lfpp_caml -lfpp$* -lapron_caml -lapron -lgmp_caml -lmpfr -lgmp -lglpk -lbigarray -lcamlidl"

fpp%.cma: fpp.cmo libfpp%.a libfpp%_caml.a
	$(OCAMLMKLIB) -o fpp$* -oc fpp$*_caml fpp.cmo -lfpp$* $(LIBS)

fpp%.cmxa fpp%.a: fpp.cmx libfpp%.a libfpp%_caml.a
	$(OCAMLMKLIB) -o fpp$* -oc fpp$*_caml fpp.cmx -lfpp$* $(LIBS)

fpp%.d.cmxa fpp%.d.a: fpp.cmx libfpp%_debug.a libfpp%_caml_debug.a
	$(OCAMLMKLIB) -o fpp$*.d -oc fpp$*_caml_debug fpp.cmx -lfpp$*_debug $(LIBS_DEBUG)

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
