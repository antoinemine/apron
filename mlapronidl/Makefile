include ../Makefile.config
include ../vars.mk

#---------------------------------------
# C part
#---------------------------------------

ICFLAGS += $(APRON_ICFLAGS) $(MP_ICFLAGS) $(ML_ICFLAGS)
LDFLAGS += $(APRON_LIFLAGS) $(MP_LIFLAGS)
CMXSINC = $(APRON_CMXSINC) -I .

#---------------------------------------
# Files
#---------------------------------------

IDLMODULES = \
scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy \
disjunction version

IDLMODULEStex = Introduction.tex \
Scalar.tex Interval.tex Coeff.tex \
Dim.tex Linexpr0.tex Lincons0.tex Generator0.tex  Texpr0.tex Tcons0.tex Texpr1.tex Tcons1.tex Manager.tex Abstract0.tex \
Var.tex Environment.tex Linexpr1.tex Lincons1.tex Generator1.tex Texpr1.tex Tcons1.tex Abstract1.tex Policy.tex Disjunction.tex \
Mpz.tex Mpq.tex Mpzf.tex Mpqf.tex Mpf.tex Gmp_random.tex Mpfr.tex Mpfrf.tex \
Box.tex Oct.tex Polka.tex Ppl.tex Pplite.tex PolkaGrid.tex T1p.tex

MLMODULES = $(IDLMODULES) apron_parser apron_lexer parser

MLSRC = $(MLMODULES:%=%.mli) $(MLMODULES:%=%.ml) apron_lexer.mll apron_parser.mly
MLINT = $(MLMODULES:%=%.cmi)
MLOBJ = $(MLMODULES:%=%.cmo)
MLOBJx = $(MLMODULES:%=%.cmx)
MLLIB_TOINSTALL = $(IDLMODULES:%=%.idl) apron.cmi apron.cma
ifneq ($(HAS_OCAMLOPT),)
MLLIB_TOINSTALL += $(call OCAMLOPT_TARGETS, apron)
endif
ifneq ($(OCAMLPACK),)
MLLIB_TOINSTALL += apron_ocamldoc.mli
endif

CCMODULES = apron_caml $(IDLMODULES:%=%_caml)
CCSRC = apron_caml.h $(CCMODULES:%=%.c)
CCLIB_TOINSTALL = libapron_caml.a
CCLIB_TOINSTALL_DEBUG = libapron_caml_debug.a
ifneq ($(HAS_SHARED),)
MLLIB_TOINSTALL += dllapron_caml.$(EXT_DLL)
endif
CCINC_TOINSTALL = apron_caml.h

LIBS = -lapron -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lm

#---------------------------------------
# Rules
#---------------------------------------

all: apron.cma libapron_caml.a libapron_caml_debug.a

ifneq ($(HAS_OCAMLOPT),)
all: $(call OCAMLOPT_TARGETS, apron apron.d)
endif

ifneq ($(HAS_SHARED),)
all: dllapron_caml.$(EXT_DLL)
endif

ifneq ($(OCAMLPACK),)
all: apron_ocamldoc.mli
endif

mldep: $(MLSRC)
	ocamldep $(MLSRC)

#---------------------------------------
# Misc rules
#---------------------------------------


dist: $(IDLMODULES:%=%.idl) $(MLSRC) $(CCSRC) macros.pl apron_caml.c apron_caml.h Makefile COPYING README mlapronidl.tex perlscript_caml.pl perlscript_c.pl introduction.odoc mlapronidl.odoc META.in META.ppl.in META.pplite.in
	(cd ..; tar zcvf mlapronidl.tgz $(^:%=mlapronidl/%))

clean:
	/bin/rm -f $(IDLMODULEStex) Apron_lexer.tex Apron_parser.tex Parser.tex
	/bin/rm -f mlapronidl.out mlapronidl.aux mlapronidl.idx mlapronidl.ilg mlapronidl.ind mlapronidl.bbl mlapronidl.blg mlapronidl.dvi mlapronidl.log mlapronidl.toc mlapronidl.ps mlapronidl.pdf
	/bin/rm -f *.o *.a *.$(EXT_DLL) *.annot *.cmi *.cmo *.cmx *.cmx[as] *.cma apron_parser.ml apron_parser.mli apron_lexer.ml
	/bin/rm -fr tmp html
	/bin/rm -f ocamldoc.[cefkimoptv]* ocamldoc.sty apron_ocamldoc.mli
	/bin/rm -f $(IDLMODULES:%=%.ml) $(IDLMODULES:%=%.mli) $(IDLMODULES:%=%_caml.c)

distclean: clean
	/bin/rm -f depend

uninstall:
ifeq ($(OCAMLFIND),)
	/bin/rm -f $(MLLIB_TOINSTALL:%=$(APRON_LIB)/%)
	/bin/rm -f $(CCLIB_TOINSTALL:%=$(APRON_LIB)/%) libapron*.$(EXT_DLL)
	/bin/rm -f $(CCINC_TOINSTALL:%=$(APRON_INCLUDE)/%)
endif

install: $(MLLIB_TOINSTALL) $(CCLIB_TOINSTALL) $(CCINC_TOINSTALL)
ifeq ($(OCAMLFIND),)
	mkdir -p $(APRON_LIB)
	mkdir -p $(APRON_INCLUDE)
	for i in $(CCLIB_TOINSTALL); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; done
	$(INSTALL) $(CCINC_TOINSTALL) $(APRON_INCLUDE)
	for i in $(MLLIB_TOINSTALL); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; done
ifneq ($(HAS_DEBUG),)
	for i in $(CCLIB_TOINSTALL_DEBUG); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; done
	for i in $(MLLIB_TOINSTALL_DEBUG); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; done
endif
endif

#---------------------------------------
# Library rules
#---------------------------------------

apron.cma: apron.cmo libapron_caml.a
	$(OCAMLMKLIB) -o apron -oc apron_caml apron.cmo $(LIBS)

apron.cmxa: apron.a
apron.a: apron.cmx libapron_caml.a
	$(OCAMLMKLIB) -o apron -oc apron_caml apron.cmx $(LIBS)

apron.d.cmxa: apron.d.a
apron.d.a: apron.cmx libapron_caml_debug.a
	$(OCAMLMKLIB) -o apron.d -oc apron_caml_debug apron.cmx $(LIBS_DEBUG)

dllapron_caml.$(EXT_DLL): libapron_caml.a

libapron_caml.a: $(CCMODULES:%=%.o)
	$(OCAMLMKLIB) $(CCMODULES:%=%.o) $(LDFLAGS) $(LIBS) -o apron_caml

libapron_caml_debug.a: $(CCMODULES:%=%_debug.o)
	$(OCAMLMKLIB) $(CCMODULES:%=%_debug.o) $(LDFLAGS) $(LIBS_DEBUG) -o apron_caml_debug

#---------------------------------------
# CAML rules
#---------------------------------------

apron.cmo: $(MLOBJ) $(MLINT)
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
	sh $(OCAMLPACK) -o $@ -title "APRON Library" -intro introduction.odoc $(MLMODULES)
	$(SED) -i -e 's/{5/{6/g;s/{4/{5/g;s/{3/{4/g;s/{2/{3/g;s/{1/{2/g' $@

mlapronidl.pdf: mlapronidl.tex introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../pplite/pplite.mli ../products/polkaGrid.mli ../taylor1plus/t1p.mli
	$(OCAMLDOC) $(OCAMLINC) -I ../newpolka -I ../ppl -I ../pplite \
	-latextitle 1,chapter -latextitle 2,section -latextitle 3,subsection -latextitle 4,subsubsection -latextitle 5,paragraph -latextitle 6,subparagraph -noheader -notrailer -latex -sepfiles introduction.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../pplite/pplite.mli ../products/polkaGrid.mli ../taylor1plus/t1p.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli
	$(PDFLATEX) mlapronidl
	$(MAKEINDEX) mlapronidl
	$(PDFLATEX) mlapronidl
	$(PDFLATEX) mlapronidl

html: mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli $(MLMODULES:%=%.mli) ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../pplite/pplite.mli ../products/polkaGrid.mli ../taylor1plus/t1p.mli
	mkdir -p html
	$(OCAMLDOC) $(OCAMLINC)  -I ../newpolka -I ../ppl -I ../pplite \
-html -d html -colorize-code -intro mlapronidl.odoc introduction.mli $(IDLMODULES:%=%.mli) parser.mli ../box/box.mli ../octagons/oct.mli ../newpolka/polka.mli ../ppl/ppl.mli ../pplite/pplite.mli ../products/polkaGrid.mli ../taylor1plus/t1p.mli $(MLGMPIDL_LIB)/mpz.mli $(MLGMPIDL_LIB)/mpq.mli $(MLGMPIDL_LIB)/gmp_random.mli $(MLGMPIDL_LIB)/mpf.mli $(MLGMPIDL_LIB)/mpfr.mli $(MLGMPIDL_LIB)/mpzf.mli $(MLGMPIDL_LIB)/mpqf.mli $(MLGMPIDL_LIB)/mpfrf.mli

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .tex .fig .c .h .o .ml .mli .cmi .cmo .cmx .idl _debug.o _prof.o _caml.c .nw

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c apron_caml.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c $< -o $@
%_debug.o: %.c apron_caml.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c $< -o $@

#---------------------------------------
# IDL generic rules
#---------------------------------------

tmp:
	mkdir -p tmp

# Generate for each %.idl: %i.ml, %i.mli, %_caml.c
%.ml %.mli %_caml.c: %.idl macros.pl perlscript_c.pl perlscript_caml.pl tmp
	cp $*.idl tmp/$*.idl
	$(CAMLIDL) -no-include -prepro "$(PERL) macros.pl" tmp/$*.idl
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

%.cmxs: %.cmxa
	$(OCAMLOPT_CMXS) $(CMXSINC) -o $@ $<

#---------------------------------------
# Dependencies
#---------------------------------------

INCLUDE_DEPEND ?= yes
ifneq ($(INCLUDE_DEPEND),yes)
  depend: $(MLMODULES:%=%.ml) $(MLMODULES:%=%.mli)
	$(OCAMLDEP) $^ > $@
else
  # Hack to avoid rebuilding depends on cleanup
  ifeq ($(filter %clean,$(MAKECMDGOALS)),)
    -include depend
  endif
endif
