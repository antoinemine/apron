include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)
#
#
PREFIX = $(APRON_PREFIX)
#

#---------------------------------------
# CAML part
#---------------------------------------

OCAMLINC = -I $(MLGMPIDL_PREFIX)/lib

#---------------------------------------
# C part
#---------------------------------------
ICFLAGS = \
-I$(MLGMPIDL_PREFIX)/include -I../apron \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_PREFIX)/lib/ocaml -I$(CAMLIDL_PREFIX)/lib/ocaml

#---------------------------------------
# Files
#---------------------------------------

IDLMODULES = \
scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy 

IDLMODULEStex = Introduction.tex \
Scalar.tex Interval.tex Coeff.tex \
Dim.tex Linexpr0.tex Lincons0.tex Generator0.tex  Texpr0.tex Tcons0.tex Texpr1.tex Tcons1.tex Manager.tex Abstract0.tex \
Var.tex Environment.tex Linexpr1.tex Lincons1.tex Generator1.tex Texpr1.tex Tcons1.tex Abstract1.tex Policy.tex \
Mpz.tex Mpq.tex Mpzf.tex Mpqf.tex Mpf.tex Gmp_random.tex Mpfr.tex Mpfrf.tex Box.tex Oct.tex Polka.tex Ppl.tex PolkaGrid.tex

MLMODULES = $(IDLMODULES) apron_parser apron_lexer parser

MLSRC = $(MLMODULES:%=%.mli) $(MLMODULES:%=%.ml) apron_lexer.mll apron_parser.mly
MLINT = $(MLMODULES:%=%.cmi)
MLOBJ = $(MLMODULES:%=%.cmo)
MLOBJx = $(MLMODULES:%=%.cmx)
MLLIB_TOINSTALL = $(IDLMODULES:%=%.idl) apron.cmi apron.cma
ifneq ($(HAS_OCAMLOPT),)
MLLIB_TOINSTALL := $(MLLIB_TOINSTALL) apron.cmxa apron.a
endif

CCMODULES = apron_caml $(IDLMODULES:%=%_caml)
CCSRC = apron_caml.h $(CCMODULES:%=%.c)
CCLIB_TOINSTALL = libapron_caml.a libapron_caml_debug.a 
ifneq ($(HAS_SHARED),)
CCLIB_TOINSTALL := $(CCLIB_TOINSTALL) \
libapron_caml.so libapron_caml_debug.so dllapron_caml.so dllapron_caml_debug.so
endif
CCINC_TOINSTALL = apron_caml.h

LIBS = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron
LIBS_DEBUG = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron_debug

#---------------------------------------
# Rules
#---------------------------------------

all: apron.cmi apron.cma libapron_caml.a libapron_caml_debug.a

ifneq ($(HAS_OCAMLOPT),)
all: apron.cmxa apron.a
endif

ifneq ($(HAS_SHARED),)
all: libapron_caml.so libapron_caml_debug.so dllapron_caml.so dllapron_caml_debug.so
endif

mldep: $(MLSRC)
	ocamldep $(MLSRC)

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(IDLMODULES:%=%.idl) apron_lexer.mll apron_lexer.mli apron_parser.mly parser.ml parser.mli apron_caml.c apron_caml.h macros.m4 Makefile COPYING README mlapronidl.tex sedscript_caml sedscript_c
	(cd ..; tar zcvf mlapronidl.tgz $(^:%=mlapronidl/%))

dist: $(IDLMODULES:%=%.idl) $(MLSRC) $(CCSRC) macros.m4 apron_caml.c apron_caml.h Makefile COPYING README mlapronidl.tex sedscript_caml sedscript_c mlapronidl.pdf html
	(cd ..; tar zcvf mlapronidl.tgz $(^:%=mlapronidl/%))

clean:
	/bin/rm -f $(IDLMODULEStex) Apron_lexer.tex Apron_parser.tex Parser.tex
	/bin/rm -f mpz.idl mpq.idl
	/bin/rm -f mlapronidl.out mlapronidl.aux mlapronidl.idx mlapronidl.ilg mlapronidl.ind mlapronidl.bbl mlapronidl.blg mlapronidl.dvi mlapronidl.log mlapronidl.toc mlapronidl.ps mlapronidl.pdf
	/bin/rm -f *.o *.a *.so *.annot *.cmi *.cmo *.cmx *.cmxa *.cma apron_parser.ml apron_parser.mli apron_lexer.ml
	/bin/rm -fr tmp html
	/bin/rm -f ocamldoc.[cefkimoptv]* ocamldoc.sty

mostlyclean: clean
	/bin/rm -f $(IDLMODULES:%=%.ml) $(IDLMODULES:%=%.mli) $(IDLMODULES:%=%_caml.c)

distclean:
	/bin/rm -f $(MLLIB_TOINSTALL:%=$(APRON_PREFIX)/lib/%)
	/bin/rm -f $(CCLIB_TOINSTALL:%=$(APRON_PREFIX)/lib/%)
	/bin/rm -f $(CCINC_TOINSTALL:%=$(APRON_PREFIX)/include/%)

install: $(MLLIB_TOINSTALL) $(CCLIB_TOINSTALL) $(CCINC_TOINSTALL)
	mkdir -p $(APRON_PREFIX)/lib
	mkdir -p $(APRON_PREFIX)/include
	for i in $(MLLIB_TOINSTALL) $(CCLIB_TOINSTALL); do if test -f $$i; then cp -f $$i $(APRON_PREFIX)/lib; fi; done
	$(INSTALL) $(CCINC_TOINSTALL) $(APRON_PREFIX)/include

#---------------------------------------
# Library rules
#---------------------------------------

apron.cma: apron.cmo libapron_caml.a
	$(OCAMLMKLIB) -ocamlc "$(OCAMLC)" -verbose -o apron -oc apron_caml apron.cmo $(LIBS)

apron.cmxa apron.a: apron.cmx libapron_caml.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT)" -verbose -o apron -oc apron_caml apron.cmx $(LIBS)

libapron_caml.a: $(CCMODULES:%=%.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml_debug.a: $(CCMODULES:%=%_debug.o)
	$(AR) rc $@ $^
	$(RANLIB) $@

libapron_caml.so: $(CCMODULES:%=%.o) ../apron/libapron.so
	$(CC) $(CFLAGS) -shared -o $@ $(CCMODULES:%=%.o) $(LIBS)

libapron_caml_debug.so: $(CCMODULES:%=%_debug.o) ../apron/libapron_debug.so
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $(CCMODULES:%=%_debug.o) $(LIBS_DEBUG)

dllapron_caml.so: libapron_caml.so
	ln -sf $^ $@
dllapron_caml_debug.so: libapron_caml_debug.so
	ln -sf $^ $@

#---------------------------------------
# CAML rules
#---------------------------------------

apron.cmo apron.cmi: $(MLOBJ) $(MLINT)
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -pack -o $@ $(MLOBJ)

apron.cmx apron.o: $(MLOBJx)
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -pack -o $@ $(MLOBJx)

apron_lexer.ml: apron_lexer.mll
	$(OCAMLLEX) $^
apron_parser.ml apron_parser.mli: apron_parser.mly
	$(OCAMLYACC) $^

#---------------------------------------
# TEX and HTML rules
#---------------------------------------

.PHONY: html

mlapronidl.pdf: mlapronidl.dvi
	$(DVIPDF) mlapronidl.dvi

mlapronidl.dvi: mlapronidl.tex introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli  ../products/polkaGrid.mli $(MLGMPIDL_PREFIX)/lib/mpz.mli $(MLGMPIDL_PREFIX)/lib/mpq.mli $(MLGMPIDL_PREFIX)/lib/gmp_random.mli $(MLGMPIDL_PREFIX)/lib/mpf.mli $(MLGMPIDL_PREFIX)/lib/mpfr.mli $(MLGMPIDL_PREFIX)/lib/mpzf.mli $(MLGMPIDL_PREFIX)/lib/mpqf.mli $(MLGMPIDL_PREFIX)/lib/mpfrf.mli
	$(OCAMLDOC) $(OCAMLINC) -I ../newpolka -I ../ppl \
	-latextitle 1,chapter -latextitle 2,section -latextitle 3,subsection -latextitle 4,subsubsection -latextitle 5,paragraph -latextitle 6,subparagraph -noheader -notrailer -latex -sepfiles introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_PREFIX)/lib/mpz.mli $(MLGMPIDL_PREFIX)/lib/mpq.mli $(MLGMPIDL_PREFIX)/lib/gmp_random.mli $(MLGMPIDL_PREFIX)/lib/mpf.mli $(MLGMPIDL_PREFIX)/lib/mpfr.mli $(MLGMPIDL_PREFIX)/lib/mpzf.mli $(MLGMPIDL_PREFIX)/lib/mpqf.mli $(MLGMPIDL_PREFIX)/lib/mpfrf.mli
	$(LATEX) mlapronidl
	$(MAKEINDEX) mlapronidl
	$(LATEX) mlapronidl
	$(LATEX) mlapronidl

html: mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_PREFIX)/lib/mpz.mli $(MLGMPIDL_PREFIX)/lib/mpq.mli $(MLGMPIDL_PREFIX)/lib/gmp_random.mli $(MLGMPIDL_PREFIX)/lib/mpf.mli $(MLGMPIDL_PREFIX)/lib/mpfr.mli $(MLGMPIDL_PREFIX)/lib/mpzf.mli $(MLGMPIDL_PREFIX)/lib/mpqf.mli $(MLGMPIDL_PREFIX)/lib/mpfrf.mli
	mkdir -p html
	$(OCAMLDOC) $(OCAMLINC)  -I ../newpolka -I ../ppl \
-html -d html -colorize-code -intro mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_PREFIX)/lib/mpz.mli $(MLGMPIDL_PREFIX)/lib/mpq.mli $(MLGMPIDL_PREFIX)/lib/gmp_random.mli $(MLGMPIDL_PREFIX)/lib/mpf.mli $(MLGMPIDL_PREFIX)/lib/mpfr.mli $(MLGMPIDL_PREFIX)/lib/mpzf.mli $(MLGMPIDL_PREFIX)/lib/mpqf.mli $(MLGMPIDL_PREFIX)/lib/mpfrf.mli

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .tex .fig .c .h .o .ml .mli .cmi .cmo .cmx .idl _debug.o _prof.o _caml.c .nw

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c apron_caml.h
	$(CC) $(CFLAGS) $(ICFLAGS) -fPIC -c $<
%_debug.o: %.c apron_caml.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -fPIC -c -o $@ $<

#---------------------------------------
# IDL generic rules
#---------------------------------------

mpz.idl: $(MLGMPIDL_PREFIX)/lib/mpz.idl
	ln -sf $^ $@
mpq.idl: $(MLGMPIDL_PREFIX)/lib/mpq.idl
	ln -sf $^ $@

# Generate for each %.idl: %i.ml, %i.mli, %_caml.c
rebuild: mpz.idl mpq.idl
	mkdir -p tmp
	for i in $(IDLMODULES); do \
		echo "module $$i"; \
		cp $${i}.idl tmp/$${i}.idl; \
		$(CAMLIDL) -no-include -prepro "$(M4) --prefix-builtins macros.m4" -I $(SRCDIR) tmp/$${i}.idl; \
		$(SED) -f sedscript_c tmp/$${i}_stubs.c >$${i}_caml.c; \
		$(SED) -f sedscript_caml tmp/$${i}.ml >$${i}.ml; \
		$(SED) -f sedscript_caml tmp/$${i}.mli >$${i}.mli; \
	done
	rm -fr tmp

#---------------------------------------
# ML generic rules
#---------------------------------------

%.cmi: %.mli
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmo: %.ml %.cmi
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmx: %.ml %.cmi
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -for-pack Apron -c $<

#---------------------------------------
# Dependencies
#---------------------------------------

depend: $(MLMODULES:%=%.ml) $(MLMODULES:%=%.mli)
	$(OCAMLDEP) $^

scalar.cmo: scalar.cmi
scalar.cmx: scalar.cmi
interval.cmo: scalar.cmi interval.cmi
interval.cmx: scalar.cmx interval.cmi
coeff.cmo: scalar.cmi interval.cmi coeff.cmi
coeff.cmx: scalar.cmx interval.cmx coeff.cmi
dim.cmo: dim.cmi
dim.cmx: dim.cmi
linexpr0.cmo: scalar.cmi interval.cmi dim.cmi coeff.cmi linexpr0.cmi
linexpr0.cmx: scalar.cmx interval.cmx dim.cmx coeff.cmx linexpr0.cmi
lincons0.cmo: scalar.cmi linexpr0.cmi lincons0.cmi
lincons0.cmx: scalar.cmx linexpr0.cmx lincons0.cmi
generator0.cmo: linexpr0.cmi generator0.cmi
generator0.cmx: linexpr0.cmx generator0.cmi
texpr0.cmo: linexpr0.cmi dim.cmi coeff.cmi texpr0.cmi
texpr0.cmx: linexpr0.cmx dim.cmx coeff.cmx texpr0.cmi
tcons0.cmo: texpr0.cmi scalar.cmi lincons0.cmi tcons0.cmi
tcons0.cmx: texpr0.cmx scalar.cmx lincons0.cmx tcons0.cmi
manager.cmo: manager.cmi
manager.cmx: manager.cmi
abstract0.cmo: texpr0.cmi tcons0.cmi manager.cmi linexpr0.cmi lincons0.cmi \
    interval.cmi generator0.cmi dim.cmi abstract0.cmi
abstract0.cmx: texpr0.cmx tcons0.cmx manager.cmx linexpr0.cmx lincons0.cmx \
    interval.cmx generator0.cmx dim.cmx abstract0.cmi
var.cmo: var.cmi
var.cmx: var.cmi
environment.cmo: var.cmi dim.cmi environment.cmi
environment.cmx: var.cmx dim.cmx environment.cmi
linexpr1.cmo: var.cmi linexpr0.cmi environment.cmi coeff.cmi linexpr1.cmi
linexpr1.cmx: var.cmx linexpr0.cmx environment.cmx coeff.cmx linexpr1.cmi
lincons1.cmo: var.cmi scalar.cmi linexpr1.cmi linexpr0.cmi lincons0.cmi \
    environment.cmi coeff.cmi abstract0.cmi lincons1.cmi
lincons1.cmx: var.cmx scalar.cmx linexpr1.cmx linexpr0.cmx lincons0.cmx \
    environment.cmx coeff.cmx abstract0.cmx lincons1.cmi
generator1.cmo: var.cmi linexpr1.cmi linexpr0.cmi generator0.cmi \
    environment.cmi coeff.cmi abstract0.cmi generator1.cmi
generator1.cmx: var.cmx linexpr1.cmx linexpr0.cmx generator0.cmx \
    environment.cmx coeff.cmx abstract0.cmx generator1.cmi
texpr1.cmo: var.cmi texpr0.cmi linexpr1.cmi environment.cmi dim.cmi coeff.cmi \
    texpr1.cmi
texpr1.cmx: var.cmx texpr0.cmx linexpr1.cmx environment.cmx dim.cmx coeff.cmx \
    texpr1.cmi
tcons1.cmo: var.cmi texpr1.cmi texpr0.cmi tcons0.cmi scalar.cmi lincons0.cmi \
    environment.cmi coeff.cmi abstract0.cmi tcons1.cmi
tcons1.cmx: var.cmx texpr1.cmx texpr0.cmx tcons0.cmx scalar.cmx lincons0.cmx \
    environment.cmx coeff.cmx abstract0.cmx tcons1.cmi
abstract1.cmo: var.cmi texpr1.cmi tcons1.cmi manager.cmi linexpr1.cmi \
    lincons1.cmi interval.cmi generator1.cmi environment.cmi abstract0.cmi \
    abstract1.cmi
abstract1.cmx: var.cmx texpr1.cmx tcons1.cmx manager.cmx linexpr1.cmx \
    lincons1.cmx interval.cmx generator1.cmx environment.cmx abstract0.cmx \
    abstract1.cmi
apron_parser.cmo: scalar.cmi lincons0.cmi interval.cmi generator0.cmi \
    coeff.cmi apron_parser.cmi
apron_parser.cmx: scalar.cmx lincons0.cmx interval.cmx generator0.cmx \
    coeff.cmx apron_parser.cmi
apron_lexer.cmo: apron_parser.cmi apron_lexer.cmi
apron_lexer.cmx: apron_parser.cmx apron_lexer.cmi
parser.cmo: var.cmi manager.cmi linexpr1.cmi lincons1.cmi lincons0.cmi \
    generator1.cmi generator0.cmi environment.cmi coeff.cmi apron_parser.cmi \
    apron_lexer.cmi abstract1.cmi parser.cmi
parser.cmx: var.cmx manager.cmx linexpr1.cmx lincons1.cmx lincons0.cmx \
    generator1.cmx generator0.cmx environment.cmx coeff.cmx apron_parser.cmx \
    apron_lexer.cmx abstract1.cmx parser.cmi
interval.cmi: scalar.cmi
coeff.cmi: scalar.cmi interval.cmi
linexpr0.cmi: dim.cmi coeff.cmi
lincons0.cmi: scalar.cmi linexpr0.cmi dim.cmi
generator0.cmi: linexpr0.cmi dim.cmi
texpr0.cmi: linexpr0.cmi dim.cmi coeff.cmi
tcons0.cmi: texpr0.cmi scalar.cmi lincons0.cmi dim.cmi
abstract0.cmi: texpr0.cmi tcons0.cmi manager.cmi linexpr0.cmi lincons0.cmi \
    interval.cmi generator0.cmi dim.cmi
environment.cmi: var.cmi dim.cmi
linexpr1.cmi: var.cmi linexpr0.cmi environment.cmi coeff.cmi
lincons1.cmi: var.cmi scalar.cmi linexpr1.cmi lincons0.cmi environment.cmi \
    coeff.cmi
generator1.cmi: var.cmi linexpr1.cmi generator0.cmi environment.cmi coeff.cmi
texpr1.cmi: var.cmi texpr0.cmi linexpr1.cmi environment.cmi dim.cmi coeff.cmi
tcons1.cmi: texpr1.cmi tcons0.cmi scalar.cmi lincons0.cmi environment.cmi
abstract1.cmi: var.cmi texpr1.cmi tcons1.cmi manager.cmi linexpr1.cmi \
    lincons1.cmi interval.cmi generator1.cmi environment.cmi abstract0.cmi
apron_parser.cmi: lincons0.cmi generator0.cmi coeff.cmi
apron_lexer.cmi: apron_parser.cmi
parser.cmi: manager.cmi linexpr1.cmi lincons1.cmi generator1.cmi \
    environment.cmi abstract1.cmi
