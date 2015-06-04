include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)
#
#
PREFIX = $(APRON_PREFIX)
#
ifneq ($(OCAMLFIND),)
MLGMPIDL_INCLUDE = $(shell $(OCAMLFIND) query gmp)
MLGMPIDL_LIB = $(MLGMPIDL_INCLUDE)
else
MLGMPIDL_INCLUDE = $(MLGMPIDL_PREFIX)/include
MLGMPIDL_LIB = $(MLGMPIDL_PREFIX)/lib
endif

#---------------------------------------
# CAML part
#---------------------------------------

OCAMLINC = -I $(MLGMPIDL_LIB)

#---------------------------------------
# C part
#---------------------------------------
ICFLAGS = \
-I$(MLGMPIDL_INCLUDE) -I../apron \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_PREFIX) -I$(CAMLIDL_PREFIX)

#---------------------------------------
# Files
#---------------------------------------

IDLMODULES = \
scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy \
disjunction

IDLMODULEStex = Introduction.tex \
Scalar.tex Interval.tex Coeff.tex \
Dim.tex Linexpr0.tex Lincons0.tex Generator0.tex  Texpr0.tex Tcons0.tex Texpr1.tex Tcons1.tex Manager.tex Abstract0.tex \
Var.tex Environment.tex Linexpr1.tex Lincons1.tex Generator1.tex Texpr1.tex Tcons1.tex Abstract1.tex Policy.tex Disjunction.tex \
Mpz.tex Mpq.tex Mpzf.tex Mpqf.tex Mpf.tex Gmp_random.tex Mpfr.tex Mpfrf.tex Box.tex Oct.tex Polka.tex Ppl.tex PolkaGrid.tex

MLMODULES = $(IDLMODULES) apron_parser apron_lexer parser

MLSRC = $(MLMODULES:%=%.mli) $(MLMODULES:%=%.ml) apron_lexer.mll apron_parser.mly
MLINT = $(MLMODULES:%=%.cmi)
MLOBJ = $(MLMODULES:%=%.cmo)
MLOBJx = $(MLMODULES:%=%.cmx)
MLLIB_TOINSTALL = $(IDLMODULES:%=%.idl) apron.cmi apron.cma
ifneq ($(HAS_OCAMLOPT),)
MLLIB_TOINSTALL += apron.cmxa apron.a
endif
ifneq ($(OCAMLPACK),)
MLLIB_TOINSTALL += apron_ocamldoc.mli
endif

CCMODULES = apron_caml $(IDLMODULES:%=%_caml)
CCSRC = apron_caml.h $(CCMODULES:%=%.c)
CCLIB_TOINSTALL = libapron_caml.a libapron_caml_debug.a
ifneq ($(HAS_SHARED),)
MLLIB_TOINSTALL += dllapron_caml.so
endif
CCINC_TOINSTALL = apron_caml.h

LIBS = -L$(APRON_PREFIX)/lib -lapron -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp -lm
LIBS_DEBUG = -L$(APRON_PREFIX)/lib -lapron_debug -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp -lm

#---------------------------------------
# Rules
#---------------------------------------

all: apron.cmi apron.cma libapron_caml.a libapron_caml_debug.a

ifneq ($(HAS_OCAMLOPT),)
all: apron.cmxa apron.a apron.d.cmxa apron.d.a
endif

ifneq ($(HAS_SHARED),)
all: dllapron_caml.so
endif

ifneq ($(OCAMLPACK),)
all: apron_ocamldoc.mli
endif

mldep: $(MLSRC)
	ocamldep $(MLSRC)

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(IDLMODULES:%=%.idl) apron_lexer.mll apron_lexer.mli apron_parser.mly parser.ml parser.mli apron_caml.c apron_caml.h macros.pl Makefile COPYING README mlapronidl.tex perlscript_caml.pl perlscript_c.pl META
	(cd ..; tar zcvf mlapronidl.tgz $(^:%=mlapronidl/%))

dist: $(IDLMODULES:%=%.idl) $(MLSRC) $(CCSRC) macros.pl apron_caml.c apron_caml.h Makefile COPYING README mlapronidl.tex perlscript_caml.pl perlscript_c.pl mlapronidl.pdf html META
	(cd ..; tar zcvf mlapronidl.tgz $(^:%=mlapronidl/%))

clean:
	/bin/rm -f $(IDLMODULEStex) Apron_lexer.tex Apron_parser.tex Parser.tex
	/bin/rm -f mlapronidl.out mlapronidl.aux mlapronidl.idx mlapronidl.ilg mlapronidl.ind mlapronidl.bbl mlapronidl.blg mlapronidl.dvi mlapronidl.log mlapronidl.toc mlapronidl.ps mlapronidl.pdf
	/bin/rm -f *.o *.a *.so *.annot *.cmi *.cmo *.cmx *.cmxa *.cma apron_parser.ml apron_parser.mli apron_lexer.ml
	/bin/rm -fr tmp html
	/bin/rm -f ocamldoc.[cefkimoptv]* ocamldoc.sty apron_ocamldoc.mli
	/bin/rm -f $(IDLMODULES:%=%.ml) $(IDLMODULES:%=%.mli) $(IDLMODULES:%=%_caml.c)

distclean: clean

uninstall:
ifeq ($(OCAMLFIND),)
	/bin/rm -f $(MLLIB_TOINSTALL:%=$(APRON_PREFIX)/lib/%)
	/bin/rm -f $(CCLIB_TOINSTALL:%=$(APRON_PREFIX)/lib/%) libapron*.so
	/bin/rm -f $(CCINC_TOINSTALL:%=$(APRON_PREFIX)/include/%)
endif

install: $(MLLIB_TOINSTALL) $(CCLIB_TOINSTALL) $(CCINC_TOINSTALL)
ifeq ($(OCAMLFIND),)
	mkdir -p $(APRON_PREFIX)/lib
	mkdir -p $(APRON_PREFIX)/include
	for i in $(CCLIB_TOINSTALL); do if test -f $$i; then cp -f $$i $(APRON_PREFIX)/lib; fi; done
	$(INSTALL) $(CCINC_TOINSTALL) $(APRON_PREFIX)/include
	for i in $(MLLIB_TOINSTALL); do if test -f $$i; then cp -f $$i $(APRON_PREFIX)/lib; fi; done
endif

#---------------------------------------
# Library rules
#---------------------------------------

apron.cma: apron.cmo libapron_caml.a
	$(OCAMLMKLIB) -ocamlc "$(OCAMLC)" -verbose -o apron -oc apron_caml apron.cmo $(LIBS) -L../apron

apron.cmxa apron.a: apron.cmx libapron_caml.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT)" -verbose -o apron -oc apron_caml apron.cmx $(LIBS) -L../apron

apron.d.cmxa apron.d.a: apron.cmx libapron_caml_debug.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT)" -verbose -o apron.d -oc apron_caml_debug apron.cmx $(LIBS_DEBUG) -L../apron

libapron_caml.a: $(CCMODULES:%=%.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml_debug.a: $(CCMODULES:%=%_debug.o)
	$(AR) rc $@ $^
	$(RANLIB) $@

dllapron_caml.so: $(CCMODULES:%=%.o) ../apron/libapron.so
	$(CC) $(CFLAGS) -shared -o $@ $(CCMODULES:%=%.o) $(LIBS) -L../apron

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
apron_ocamldoc.mli: introduction.odoc $(MLMODULES:%=%.mli)
	$(OCAMLPACK) -o apron_ocamldoc -title "APRON library" -intro introduction.odoc -intf $(MLMODULES)
	$(SED) -i -e 's/{5/{6/g;s/{4/{5/g;s/{3/{4/g;s/{2/{3/g;s/{1/{2/g' $@

mlapronidl.pdf: mlapronidl.dvi
	$(DVIPDF) mlapronidl.dvi

mlapronidl.dvi: mlapronidl.tex introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli  ../products/polkaGrid.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli
	$(OCAMLDOC) $(OCAMLINC) -I ../newpolka -I ../ppl \
	-latextitle 1,chapter -latextitle 2,section -latextitle 3,subsection -latextitle 4,subsubsection -latextitle 5,paragraph -latextitle 6,subparagraph -noheader -notrailer -latex -sepfiles introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli
	$(LATEX) mlapronidl
	$(MAKEINDEX) mlapronidl
	$(LATEX) mlapronidl
	$(LATEX) mlapronidl

html: mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli
	mkdir -p html
	$(OCAMLDOC) $(OCAMLINC)  -I ../newpolka -I ../ppl \
-html -d html -colorize-code -intro mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../products/polkaGrid.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli

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

# Generate for each %.idl: %i.ml, %i.mli, %_caml.c
%.ml %.mli %_caml.c: %.idl macros.pl perlscript_c.pl perlscript_caml.pl
	mkdir -p tmp
	cp $*.idl tmp/$*.idl
	$(CAMLIDL) -no-include -prepro "$(PERL) macros.pl" -I $(SRCDIR) tmp/$*.idl
	$(PERL) perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml;
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

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
texpr1.cmo: var.cmi texpr0.cmi linexpr1.cmi environment.cmi coeff.cmi \
    texpr1.cmi
texpr1.cmx: var.cmx texpr0.cmx linexpr1.cmx environment.cmx coeff.cmx \
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
policy.cmo: tcons1.cmi tcons0.cmi manager.cmi lincons1.cmi lincons0.cmi \
    environment.cmi abstract1.cmi abstract0.cmi policy.cmi
policy.cmx: tcons1.cmx tcons0.cmx manager.cmx lincons1.cmx lincons0.cmx \
    environment.cmx abstract1.cmx abstract0.cmx policy.cmi
disjunction.cmo: manager.cmi lincons0.cmi abstract0.cmi disjunction.cmi
disjunction.cmx: manager.cmx lincons0.cmx abstract0.cmx disjunction.cmi
apron_parser.cmo: var.cmi texpr1.cmi tcons0.cmi scalar.cmi lincons0.cmi \
    interval.cmi generator0.cmi coeff.cmi apron_parser.cmi
apron_parser.cmx: var.cmx texpr1.cmx tcons0.cmx scalar.cmx lincons0.cmx \
    interval.cmx generator0.cmx coeff.cmx apron_parser.cmi
apron_lexer.cmo: texpr1.cmi apron_parser.cmi apron_lexer.cmi
apron_lexer.cmx: texpr1.cmx apron_parser.cmx apron_lexer.cmi
parser.cmo: var.cmi texpr1.cmi tcons1.cmi manager.cmi linexpr1.cmi \
    lincons1.cmi lincons0.cmi generator1.cmi generator0.cmi environment.cmi \
    coeff.cmi apron_parser.cmi apron_lexer.cmi abstract1.cmi parser.cmi
parser.cmx: var.cmx texpr1.cmx tcons1.cmx manager.cmx linexpr1.cmx \
    lincons1.cmx lincons0.cmx generator1.cmx generator0.cmx environment.cmx \
    coeff.cmx apron_parser.cmx apron_lexer.cmx abstract1.cmx parser.cmi
scalar.cmi:
interval.cmi: scalar.cmi
coeff.cmi: scalar.cmi interval.cmi
dim.cmi:
linexpr0.cmi: dim.cmi coeff.cmi
lincons0.cmi: scalar.cmi linexpr0.cmi dim.cmi
generator0.cmi: linexpr0.cmi dim.cmi
texpr0.cmi: linexpr0.cmi dim.cmi coeff.cmi
tcons0.cmi: texpr0.cmi scalar.cmi lincons0.cmi dim.cmi
manager.cmi:
abstract0.cmi: texpr0.cmi tcons0.cmi manager.cmi linexpr0.cmi lincons0.cmi \
    interval.cmi generator0.cmi dim.cmi
var.cmi:
environment.cmi: var.cmi dim.cmi
linexpr1.cmi: var.cmi linexpr0.cmi environment.cmi coeff.cmi
lincons1.cmi: var.cmi scalar.cmi linexpr1.cmi lincons0.cmi environment.cmi \
    coeff.cmi
generator1.cmi: var.cmi linexpr1.cmi generator0.cmi environment.cmi coeff.cmi
texpr1.cmi: var.cmi texpr0.cmi linexpr1.cmi environment.cmi coeff.cmi
tcons1.cmi: texpr1.cmi tcons0.cmi scalar.cmi lincons0.cmi environment.cmi
abstract1.cmi: var.cmi texpr1.cmi tcons1.cmi manager.cmi linexpr1.cmi \
    lincons1.cmi interval.cmi generator1.cmi environment.cmi abstract0.cmi
policy.cmi: tcons1.cmi tcons0.cmi manager.cmi lincons1.cmi lincons0.cmi \
    environment.cmi abstract1.cmi abstract0.cmi
disjunction.cmi: manager.cmi lincons0.cmi abstract0.cmi
apron_parser.cmi: texpr1.cmi tcons0.cmi lincons0.cmi generator0.cmi coeff.cmi
apron_lexer.cmi: apron_parser.cmi
parser.cmi: texpr1.cmi tcons1.cmi manager.cmi linexpr1.cmi lincons1.cmi \
    generator1.cmi environment.cmi abstract1.cmi
