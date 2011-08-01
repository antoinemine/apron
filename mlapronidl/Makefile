
include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)

SITE-LIB = $(shell $(OCAMLFIND) printconf destdir)
PKG-NAME = apron
SITE-LIB-PKG = $(SITE-LIB)/$(PKG-NAME)
REQ_PKG = bigarray,str,gmp

SITE-LIB-gmp = $(shell $(OCAMLFIND) query gmp)

#---------------------------------------
# CAML part
#---------------------------------------

OCAMLCCOPT = -ccopt -L$(SITE-LIB-PKG)

#---------------------------------------
# C part
#---------------------------------------
ICFLAGS = \
-I$(SITE-LIB-gmp) \
-I../num \
-I../apron \
-I../box -I../polka \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_PREFIX_LIB) -I$(CAMLIDL_PREFIX)/lib/ocaml

LDFLAGS = -L$(APRON_PREFIX)/lib/apron -L.. -L$(CAML_PREFIX_LIB) -L$(CAMLIDL_PREFIX)/lib/ocaml -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib \


#---------------------------------------
# Files
#---------------------------------------

TMPL = $(MLAPRONIDL_NUMTYPES)
YYY = expr cons gen

IDL_ORIG_MODULES_1 = dim var environment common
IDL_ORIG_MODULES_2 = coeff texpr0 tcons0 manager abstract0 
IDL_ORIG_MODULES = $(IDL_ORIG_MODULES_1) $(IDL_ORIG_MODULES_2)
IDL_TMPL_MODULES_XXX = numXXX boundXXX eitvXXX
IDL_TMPL_MODULES_yyyXXX = linyyyXXX
IDL_TMPL_MODULES_tmpl_1 = conv
IDL_TMPL_MODULES_tmpl_2 = box polka
IDL_TMPL_MODULES_tmpl = $(IDL_TMPL_MODULES_tmpl_1) $(IDL_TMPL_MODULES_tmpl_2)
IDL_TMPL_MODULES_yyy = linyyy0
IDL_TMPL_MODULES = \
$(IDL_TMPL_MODULES_XXX) \
$(IDL_TMPL_MODULES_yyyXXX) \
$(IDL_TMPL_MODULES_tmpl) \
$(IDL_TMPL_MODULES_yyy )

IDL_GENERATED_MODULES_1 = \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(IDL_TMPL_MODULES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(foreach T,$(TMPL),$(subst XXX,$(T),$(IDL_TMPL_MODULES_yyyXXX))))) \
$(IDL_TMPL_MODULES_tmpl_1:%.tmpl=%) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(IDL_TMPL_MODULES_yyy))) 
IDL_GENERATED_MODULES_2 = $(IDL_TMPL_MODULES_tmpl_2:%.tmpl=%)
IDL_GENERATED_MODULES = $(IDL_GENERATED_MODULES_1) $(IDL_GENERATED_MODULES_2)

IDL_MODULES_1 = $(IDL_ORIG_MODULES_1) $(IDL_GENERATED_MODULES_1) $(IDL_ORIG_MODULES_2) 
IDL_MODULES_2 = $(IDL_GENERATED_MODULES_2)
IDL_MODULES = $(IDL_MODULES_1) $(IDL_MODULES_2)

IDL_SRC_MODULES = $(IDL_ORIG_MODULES) $(IDL_TMPL_MODULES)

ML_TMPL_MODULES_tmpl = parser
ML_TMPL_MODULES_yyy = linyyy1
ML_TMPL_MODULES = $(ML_TMPL_MODULES_tmpl) $(ML_TMPL_MODULES_yyy)
ML_ORIG_MODULES = texpr1 tcons1 abstract1 apron_parser apron_lexer
ML_GENERATED_MODULES_1 = \
$(IDL_MODULES_1) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(ML_TMPL_MODULES_yyy)))
ML_GENERATED_MODULES_2 = \
$(ML_TMPL_MODULES_tmpl:%.tmpl=%) \
$(IDL_MODULES_2)
ML_GENERATED_MODULES = $(ML_GENERATED_MODULES_1) $(ML_GENERATED_MODULES_2)
ML_MODULES = $(ML_GENERATED_MODULES_1) $(ML_ORIG_MODULES) $(ML_GENERATED_MODULES_2)
ML_SRC_MODULES = $(ML_ORIG_MODULES) $(ML_TMPL_MODULES)

C_ORIG_FILES = 
C_TMPL_FILES_tmpl = apron_caml.c.tmpl 
C_GENERATED_FILES = $(IDL_MODULES:%=%_caml.c) $(C_TMPL_FILES_tmpl:%.c.tmpl=%.c)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES_tmpl)

H_ORIG_FILES = 
H_TMPL_FILES_tmpl = apron_caml.h.tmpl
H_GENERATED_FILES = $(H_TMPL_FILES_tmpl:%.h.tmpl=%.h)
H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES_tmpl)

C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES) 
H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES) 

C_MODULES = $(C_FILES:%.c=%)

#scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1

#IDLMODULEStex = Introduction.tex \
Scalar.tex Interval.tex Coeff.tex \
Dim.tex Linexpr0.tex Lincons0.tex Generator0.tex  Texpr0.tex Tcons0.tex Texpr1.tex Tcons1.tex Manager.tex Abstract0.tex \
Var.tex Environment.tex Linexpr1.tex Lincons1.tex Generator1.tex Texpr1.tex Tcons1.tex Abstract1.tex \
Mpz.tex Mpq.tex Mpzf.tex Mpqf.tex Mpf.tex Gmp_random.tex Mpfr.tex Mpfrf.tex Box.tex Oct.tex Polka.tex Ppl.tex PolkaGrid.tex

ML_LIB_ALL = apron.cma apron.callback.cma
C_LIB_ALL = libapron_caml.a libapron_caml.callback.a
ifneq ($(HAS_OCAMLOPT),)
ML_LIB_ALL += \
	apron.cmx apron.cmxa apron.a \
	apron.callback.cmxa apron.callback.a
endif
ifneq ($(HAS_SHARED),)
C_LIB_ALL += dllapron_caml.so dllapron_caml.callback.so
endif

ML_LIB_DEBUG = 
C_LIB_DEBUG = libapron_caml.d.a libapron_caml.callback.d.a
ifneq ($(HAS_OCAMLOPT),)
ML_LIB_DEBUG += \
	apron.cmx apron.d.cmxa apron.d.a \
	apron.callback.d.cmxa apron.callback.d.a
endif

ML_LIB_PROF = 
C_LIB_PROF = libapron_caml.p.a libapron_caml.callback.p.a
ifneq ($(HAS_OCAMLOPT),)
ML_LIB_PROF += \
	apron.p.cmx apron.p.cmxa apron.p.a \
	apron.callback.p.cmxa apron.callback.p.a
endif

FILES_TOINSTALL = \
	META \
	apron_caml.h \
	$(IDL_MODULES:%=%.idl) \
	apron.cmi \
	$(ML_LIB_ALL) $(C_LIB_ALL) \
	$(ML_LIB_DEBUG) $(C_LIB_DEBUG) \
	$(ML_LIB_PROF) $(C_LIB_PROF)

#---------------------------------------
# Rules
#---------------------------------------

all: src apron.cmi $(ML_LIB_ALL) $(C_LIB_ALL)
debug: src apron.cmi $(ML_LIB_DEBUG) $(C_LIB_DEBUG)
prof: src apron.cmi $(ML_LIB_PROF) $(C_LIB_PROF)

essai:
	echo "$(ICFLAGS)"

src: $(IDL_MODULES:%=%.idl) $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli) $(C_FILES) $(H_FILES)

# Example of compilation command with ocamlfind
%.byte: %.ml
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -o $@ $*.ml -package apron -linkpkg
%.opt: %.ml 
	$(OCAMLFIND) ocamlopt -verbose $(OCAMLOPTFLAGS) -o $@ $*.ml -package apron -linkpkg

# CAML rules
apron.cma: apron.cmo $(C_LIB_ALL)
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmo \
	-dllib -lapron_caml \
	$(OCAMLCCOPT) -cclib -lapron_caml -cclib -lapron
apron.callback.cma: apron.cmo $(C_LIB_ALL)
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmo \
	-dllib -lapron_caml.callback \
	$(OCAMLCCOPT) -cclib -lapron_caml.callback -cclib -lapron

apron.cmxa: apron.cmx $(C_LIB_ALL)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml -cclib -lapron 
apron.d.cmxa: $(MLMODULES:%=%.d.cmx) $(C_LIB_DEBUG)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.d -cclib -lapron.d
apron.p.cmxa: apron.p.cmx $(C_LIB_PROF)
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.p.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.p -cclib -lapron.p
apron.callback.cmxa: apron.cmx $(C_LIB_ALL)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.callback -cclib -lapron 
apron.callback.d.cmxa: $(MLMODULES:%=%.d.cmx) $(C_LIB_DEBUG)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.callback.d -cclib -lapron.d
apron.callback.p.cmxa: $(MLMODULES:%=%.p.cmx) $(C_LIB_PROF)
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.callback.p -cclib -lapron.p

# C rules
libapron_caml.a: $(C_MODULES:%=%.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml.d.a: $(C_MODULES:%=%.d.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml.p.a: $(C_MODULES:%=%.p.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml.callback.a: $(C_MODULES:%=%.callback.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml.callback.d.a: $(C_MODULES:%=%.callback.d.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
libapron_caml.callback.p.a: $(C_MODULES:%=%.callback.p.o)
	$(AR) rc $@ $^
	$(RANLIB) $@
dllapron_caml.so: $(C_MODULES:%=%.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^ -lapron
dllapron_caml.callback.so: $(C_MODULES:%=%.callback.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^ -lapron

install: $(FILES_TOINSTALL)
	$(OCAMLFIND) remove $(PKG-NAME)
	$(OCAMLFIND) install $(PKG-NAME) $^

uninstall:
	$(OCAMLFIND) remove $(PKG-NAME)

clean:
	/bin/rm -f $(IDLMODULEStex) Apron_lexer.tex Apron_parser.tex Parser.tex
	/bin/rm -f mlapronidl.out mlapronidl.aux mlapronidl.idx mlapronidl.ilg mlapronidl.ind mlapronidl.bbl mlapronidl.blg mlapronidl.dvi mlapronidl.log mlapronidl.toc mlapronidl.ps mlapronidl.pdf
	/bin/rm -f *.o *.a *.so *.cmi *.cmo *.cmx *.cmxa *.cma *.annot apron_parser.ml apron_parser.mli apron_parser.output apron_lexer.ml
	/bin/rm -fr tmp html
	/bin/rm -f ocamldoc.[cefkimoptv]* ocamldoc.sty

distclean: clean
	/bin/rm -f $(IDL_GENERATED_MODULES:%=%.idl) $(H_GENERATED_FILES) $(C_GENERATED_FILES) $(ML_GENERATED_MODULES:%=%.ml) $(ML_GENERATED_MODULES:%=%.mli) mpz.idl mpq.idl mpf.idl mpfr.idl
	for i in $(APRON_NUMTYPES); do /bin/rm -f `find . -name "*$$i.*"`; done  

#---------------------------------------
# Library rules
#---------------------------------------

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------


#-----------------------------------
# IDL
#-----------------------------------

polka_caml.c polka.ml polka.mli: polka.idl mpz.idl mpq.idl mpf.idl mpfr.idl sedscript_caml sedscript_c sedscript_polka_caml
	@mkdir -p tmp
	@echo "module polka" 
	@cp polka.idl tmp/polka.idl 
	$(CAMLIDL) -no-include -prepro cpp -I $(SRCDIR) tmp/polka.idl 
	$(SED) -f sedscript_c tmp/polka_stubs.c >polka_caml.c 
	$(SED) -f sedscript_caml -f sedscript_polka_caml tmp/polka.ml | ../cleanupidl.pl - >polka.ml 
	$(SED) -f sedscript_caml -f sedscript_polka_caml tmp/polka.mli | ../cleanupidl.pl - >polka.mli

%_caml.c %.ml %.mli: %.idl mpz.idl mpq.idl mpf.idl mpfr.idl sedscript_caml sedscript_c
	@mkdir -p tmp
	@echo "module $*" 
	@cp $*.idl tmp/$*.idl 
	$(CAMLIDL) -no-include -prepro cpp -I $(SRCDIR) tmp/$*.idl 
	$(SED) -f sedscript_c tmp/$*_stubs.c >$*_caml.c 
	$(SED) -f sedscript_caml tmp/$*.ml | ../cleanupidl.pl - >$*.ml 
	$(SED) -f sedscript_caml tmp/$*.mli | ../cleanupidl.pl - >$*.mli 

mpz.idl: $(SITE-LIB-gmp)/mpz.idl
	ln -s $^ $@
mpq.idl: $(SITE-LIB-gmp)/mpq.idl
	ln -s $^ $@
mpf.idl: $(SITE-LIB-gmp)/mpf.idl
	ln -s $^ $@
mpfr.idl: $(SITE-LIB-gmp)/mpfr.idl
	ln -s $^ $@

#-----------------------------------
# C
#-----------------------------------

%.o: %.c apron_caml.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
%.d.o: %.c apron_caml.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
%.p.o: %.c apron_caml.h
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<
%.callback.o: %.c apron_caml.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DMLAPRONIDL_CALLBACK -c -o $@ $<
%.callback.d.o: %.c apron_caml.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DMLAPRONIDL_CALLBACK -c -o $@ $<
%.callback.p.o: %.c apron_caml.h
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -DMLAPRONIDL_CALLBACK -c -o $@ $<

#-----------------------------------
# CAML
#-----------------------------------

apron_parser.ml apron_parser.mli: apron_parser.mly
	$(OCAMLYACC) -v $^
apron_lexer.ml: apron_lexer.mll
	$(OCAMLLEX) $^

apron.cmo apron.cmi: $(ML_MODULES:%=%.cmo) $(ML_MODULES:%=%.cmi)
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) \
	-pack -o $@ $(ML_MODULES:%=%.cmo)

apron.o apron.cmx: $(ML_MODULES:%=%.cmx)
	$(OCAMLFIND) ocamlopt $(OCAMLFLAGS) -package $(REQ_PKG) \
	-pack -o $@ $(ML_MODULES:%=%.cmx)

apron.p.o apron.p.cmx: $(ML_MODULES:%=%.p.cmx)
	$(OCAMLFIND) ocamlopt -p $(OCAMLFLAGS) -package $(REQ_PKG) \
	-pack -o $@ $(ML_MODULES:%=%.p.cmx)

%.cmi: %.mli
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -c $<

%.cmo: %.ml %.cmi
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -c $<

%.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -for-pack Apron -c $<

%.p.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -package $(REQ_PKG) -for-pack Apron -c -o $@ $<

#-----------------------------------
# Templates
#-----------------------------------

%.idl: %.idl.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	MLAPRONIDL_NUMTYPES="$(MLAPRONIDL_NUMTYPES)" BOX_NUMTYPES="$(BOX_NUMTYPES)" POLKA_NUMTYPES="$(POLKA_NUMTYPES)" ../macros.pl $(SRCDIR)/$< >>$@
%.ml: %.ml.tmpl
	@echo "(* GENERATED FROM $<, DO NOT MODIFY *)" >$@
	@echo "# 1 \"$(SRCDIR)/$<\"" >>$@
	MLAPRONIDL_NUMTYPES="$(MLAPRONIDL_NUMTYPES)" ../macros.pl $(SRCDIR)/$< | $(SED) -e "s/#line \([0-9]*\) \"\([^\"]*\)\"/# \1 \"\2\"/g" >>$@
%.mli: %.mli.tmpl
	@echo "(* GENERATED FROM $<, DO NOT MODIFY *)" >$@
	@echo "# 1 \"$(SRCDIR)/$<\"" >>$@
	MLAPRONIDL_NUMTYPES="$(MLAPRONIDL_NUMTYPES)" ../macros.pl $(SRCDIR)/$< | $(SED) -e "s/#line \([0-9]*\) \"\([^\"]*\)\"/# \1 \"\2\"/g" >>$@
%.h: %.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	MLAPRONIDL_NUMTYPES="$(MLAPRONIDL_NUMTYPES)" BOX_NUMTYPES="$(BOX_NUMTYPES)" POLKA_NUMTYPES="$(POLKA_NUMTYPES)" ../macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	MLAPRONIDL_NUMTYPES="$(MLAPRONIDL_NUMTYPES)" BOX_NUMTYPES="$(BOX_NUMTYPES)" POLKA_NUMTYPES="$(POLKA_NUMTYPES)" ../macros.pl $(SRCDIR)/$< >>$@

define generate-idl-XXX
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	$(SED) -e "s/XXX/$(1)/g; s/#line \([0-9]*\) \"$$@\"/#line \1 \"$$<\"/g" $$< >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(IDL_TMPL_MODULES_XXX:%=%.idl),$(eval $(call generate-idl-XXX,$(T),$(F)))))

define generate-idl-yyyXXX
$(subst yyy,$(1),$(subst XXX,$(2),$(3))): $(3)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g; s/XXX/$(2)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach T,$(TMPL),$(foreach F,$(IDL_TMPL_MODULES_yyyXXX:%=%.idl),$(eval $(call generate-idl-yyyXXX,$(Y),$(T),$(F))))))

define generate-idl-yyy
$(subst yyy,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g;s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach F,$(IDL_TMPL_MODULES_yyy:%=%.idl),$(eval $(call generate-idl-yyy,$(Y),$(F)))))

define generate-ml-yyy
$(subst yyy,$(1),$(2)): $(2)
	@echo "(* GENERATED FROM $$<, DO NOT MODIFY *)" >$$@
	$(SED) -e "s/yyy/$(1)/g;" $(SRCDIR)/$$< | cpp -P | $(SED) -e "s/# \([0-9]*\) \"<stdin>\"/# \1 \"$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach F,$(ML_TMPL_MODULES_yyy:%=%.mli) $(ML_TMPL_MODULES_yyy:%=%.ml),$(eval $(call generate-ml-yyy,$(Y),$(F)))))

#---------------------------------------
# Dependencies
#---------------------------------------

depend: $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli)
	$(OCAMLFIND) ocamldep $^ >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES) $(ML_SRC_MODULES:%=%.ml) $(ML_SRC_MODULES:%=%.mli) $(IDL_SRC_MODULES:%=%.idl)
	etags $^

