include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)

SITE-LIB = $(shell $(OCAMLFIND) printconf destdir)
PKG-NAME = apron
SITE-LIB-PKG = $(SITE-LIB)/$(PKG-NAME)
REQ_PKG = bigarray,gmp

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
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_PREFIX)/lib/ocaml -I$(CAMLIDL_PREFIX)/lib/ocaml

LDFLAGS = \
-L$(APRON_PREFIX)/lib \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(CAML_PREFIX)/lib/ocaml -L$(CAMLIDL_PREFIX)/lib/ocaml

#---------------------------------------
# Files
#---------------------------------------

TMPL = MPZ #MPQ MPFR D
YYY = expr cons gen

IDL_ORIG_MODULES = num_internal dim var environment
IDL_TMPL_MODULES_XXX = numXXX boundXXX eitvXXX
IDL_TMPL_MODULES_yyyXXX = linyyyXXX
IDL_GENERATED_MODULES = \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(IDL_TMPL_MODULES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(foreach T,$(TMPL),$(subst XXX,$(T),$(IDL_TMPL_MODULES_yyyXXX)))))

IDL_MODULES = $(IDL_ORIG_MODULES) $(IDL_GENERATED_MODULES)

ML_ORIG_MODULES = #apron_parser apron_lexer parser
ML_GENERATED_MODULES = $(IDL_MODULES) 
ML_MODULES = $(ML_GENERATED_MODULES) $(ML_ORIG_MODULES)

C_ORIG_FILES = 
C_TMPL_FILES_tmpl = apron_caml.c.tmpl
C_GENERATED_FILES = $(IDL_MODULES:%=%_caml.c) $(C_TMPL_FILES_tmpl:%.c.tmpl=%.c)

H_ORIG_FILES = 
H_TMPL_FILES_tmpl = apron_caml.h.tmpl
H_GENERATED_FILES = $(H_TMPL_FILES_tmpl:%.h.tmpl=%.h)

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

ML_LIB = $(ML_MODULES:%=%.cmi) apron.cma
ifneq ($(HAS_OCAMLOPT),)
ML_LIB += \
	$(ML_MODULES:%=%.cmx) apron.cmxa apron.a \
	$(ML_MODULES:%=%.p.cmx) apron.p.cmxa apron.p.a
endif

C_LIB = libapron_caml.a libapron_caml.p.a 
ifneq ($(HAS_SHARED),)
CCLIB += dllapron_caml.so
endif

FILES_TOINSTALL = \
	META \
	apron_caml.h \
	$(IDL_MODULES:%=%.idl) \
	$(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli) \
	$(ML_LIB) $(C_LIB)


#---------------------------------------
# Rules
#---------------------------------------

all: $(FILES_TOINSTALL)

src: 
	echo "$(C_FILES) $(H_FILES) $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli)"
	echo "$(IDL_MODULES)"

# Example of compilation command with ocamlfind
%.byte: %.ml
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -o $@ $*.ml \
	-package apron -linkpkg
%.opt: %.ml 
	$(OCAMLFIND) ocamlopt -verbose $(OCAMLOPTFLAGS) -o $@ $*.ml \
	-package apron -linkpkg

# CAML rules
apron.cma: apron.cmo $(C_LIB)
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -a -o $@ \
	apron.cmo \
	-dllib -lapron_caml \
	$(OCAMLCCOPT) -cclib -lapron_caml -cclib -lapron

apron.cmxa: apron.cmx $(C_LIB)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ \
	apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml -cclib -lapron 
apron.p.cmxa: apron.p.cmx $(C_LIB)
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ \
	apron.p.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.p -cclib -lapron.p
apron.d.cmxa: $(MLMODULES:%=%.cmx) $(C_LIB)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -a -o $@ \
	apron.cmx \
	$(OCAMLCCOPT) -cclib -lapron_caml.d -cclib -lapron.d

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
dllapron_caml.so: $(C_MODULES:%=%.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^ -lapron

META: Makefile
	/bin/rm -f META
	echo "description = \"OCaml Interface to APRON library\"" >META
	echo "version = \"2.0.0\"" >>META
	echo "requires = \"$(REQ_PKG)\"" >>META
	echo "archive(byte) = \"apron.cma\"" >>META
	echo "archive(native) = \"apron.cmxa\"" >>META
	echo "archive(native,debug) = \"apron.d.cmxa\"" >>META
	echo "archive(native,gprof) = \"apron.p.cmxa\"" >>META

install: $(FILES_TOINSTALL)
	$(OCAMLFIND) remove $(PKG-NAME)
	$(OCAMLFIND) install $(PKG-NAME) $^

uninstall:
	$(OCAMLFIND) remove $(PKG-NAME)

clean:
	/bin/rm -f $(IDLMODULEStex) Apron_lexer.tex Apron_parser.tex Parser.tex
	/bin/rm -f mlapronidl.out mlapronidl.aux mlapronidl.idx mlapronidl.ilg mlapronidl.ind mlapronidl.bbl mlapronidl.blg mlapronidl.dvi mlapronidl.log mlapronidl.toc mlapronidl.ps mlapronidl.pdf
	/bin/rm -f *.o *.a *.so *.cmi *.cmo *.cmx *.cmxa *.cma *.annot apron_parser.ml apron_parser.mli apron_lexer.ml
	/bin/rm -fr tmp html
	/bin/rm -f ocamldoc.[cefkimoptv]* ocamldoc.sty

distclean:
	/bin/rm -f $(IDL_GENERATED_MODULES:%=%.idl) $(H_GENERATED_FILES) $(C_GENERATED_FILES) $(ML_GENERATED_MODULES:%=%.ml) $(ML_GENERATED_MODULES:%=%.mli) mpz.idl mpq.idl mpf.idl mpfr.idl

#---------------------------------------
# Library rules
#---------------------------------------

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .tex .fig .c .h .o .ml .mli .cmi .cmo .cmx .idl _debug.o _prof.o _caml.c .nw

#-----------------------------------
# IDL
#-----------------------------------

$(IDL_MODULES:%=%_caml.c) $(IDL_MODULES:%=%.ml) $(IDL_MODULES:%=%.mli): $(IDL_MODULES:%=%.idl) mpz.idl mpq.idl mpf.idl mpfr.idl sedscript_caml sedscript_c
	mkdir -p tmp
	for i in $(IDL_MODULES); do \
		echo "module $$i"; \
		cp $${i}.idl tmp/$${i}.idl; \
		$(CAMLIDL) -no-include -prepro cpp -I $(SRCDIR) tmp/$${i}.idl; \
		$(SED) -f sedscript_c tmp/$${i}_stubs.c >$${i}_caml.c; \
		$(SED) -f sedscript_caml tmp/$${i}.ml >$${i}.ml; \
		$(SED) -f sedscript_caml tmp/$${i}.mli >$${i}.mli; \
	done

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
%.p.o: %.c apron_caml.h
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<
%.d.o: %.c apron_caml.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# CAML
#-----------------------------------

apron.cmo apron.cmi: $(ML_MODULES:%=%.cmo) $(ML_MODULES:%=%.cmi)
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) \
	-pack -o $@ $(ML_MODULES:%=%.cmo)

apron.o apron.cmx: $(ML_MODULES:%=%.cmx)
	$(OCAMLFIND) ocamlopt $(OCAMLFLAGS) -package $(REQ_PKG) \
	-pack -o $@ $(ML_MODULES:%=%.cmx)

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

%.h: %.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< >>$@

define generate-file-XXX
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	$(SED) -e "s/XXX/$(1)/g; s/#line \([0-9]*\) \"$$@\"/#line \1 \"$$<\"/g" $$< >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(IDL_TMPL_MODULES_XXX:%=%.idl),$(eval $(call generate-file-XXX,$(T),$(F)))))

define generate-file-yyyXXX
$(subst yyy,$(1),$(subst XXX,$(2),$(3))): $(3)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	./macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g; s/XXX/$(2)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach T,$(TMPL),$(foreach F,$(IDL_TMPL_MODULES_yyyXXX:%=%.idl),$(eval $(call generate-file-yyyXXX,$(Y),$(T),$(F))))))

#---------------------------------------
# Dependencies
#---------------------------------------

depend: $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli)
	$(OCAMLDEP) $^
