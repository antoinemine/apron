include ../Makefile.config

PREFIX = $(APRON_PREFIX)

SRCDIR = $(shell pwd)

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I$(MLGMPIDL_PREFIX)/include \
-I../num \
-I../itv \
-I../apron \
-I../mlapronidl \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(CAMLIDL_PREFIX)/lib/ocaml -I$(CAML_PREFIX)/lib/ocaml

# Caml
OCAMLINC = -I $(MLGMPIDL_PREFIX)/lib -I ../mlapronidl

#---------------------------------------
# Files
#---------------------------------------

CCMODULES = box_internal box_representation box_constructor box_meetjoin box_assign box_resize box_otherops box_policy
CCSRC = box_config.h box.h $(CCMODULES:%=%.h) $(CCMODULES:%=%.c)

IDLDEPMODULES = \
scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy 
IDLDEP = $(IDLDEPMODULES:%=%.idl)

CCINC_TO_INSTALL = box.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = \
libboxMPQ.a libboxD.a libboxMPFR.a \
libboxMPQ_debug.a libboxD_debug.a libboxMPFR_debug.a
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL := $(CCLIB_TO_INSTALL) \
libboxMPQ.so libboxD.so libboxMPFR.so \
libboxMPQ_debug.so libboxD_debug.so libboxMPFR_debug.so
endif
CAML_TO_INSTALL = \
box.idl box.ml box.mli box.cmi \
boxMPQ.cma \
boxD.cma \
boxMPFR.cma \
libboxMPQ_caml.a libboxD_caml.a libboxMPFR_caml.a \
libboxMPQ_caml_debug.a libboxD_caml_debug.a libboxMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAML_TO_INSTALL := $(CAML_TO_INSTALL) \
boxMPQ.cmxa boxMPQ.a \
boxD.cmxa boxD.a \
boxMPFR.cmxa boxMPFR.a
endif
ifneq ($(HAS_SHARED),)
CAML_TO_INSTALL := $(CAML_TO_INSTALL) \
libboxMPQ_caml.so libboxD_caml.so libboxMPFR_caml.so \
libboxMPQ_caml_debug.so libboxD_caml_debug.so libboxMPFR_caml_debug.so \
dllboxMPQ_caml.so dllboxD_caml.so dllboxMPFR_caml.so \
dllboxMPQ_caml_debug.so dllboxD_caml_debug.so dllboxMPFR_caml_debug.so
endif

LIBS = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron
LIBS_DEBUG = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron_debug

#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allMPQ allD allMPFR

allMPQ: libboxMPQ.a libboxMPQ_debug.a
allD: libboxD.a libboxD_debug.a
allMPFR: libboxMPFR.a libboxMPFR_debug.a
ifneq ($(HAS_SHARED),)
allMPQ: libboxMPQ.so libboxMPQ_debug.so
allD: libboxD.so libboxD_debug.so
allMPFR: libboxMPFR.so libboxMPFR_debug.so
endif

ml: box.mli box.ml box.cmi mlMPQ mlD mlMPFR

mlMPQ: boxMPQ.cma libboxMPQ_caml.a libboxMPQ_caml_debug.a 
mlD: boxD.cma libboxD_caml.a libboxD_caml_debug.a
mlMPFR: boxMPFR.cma libboxMPFR_caml.a libboxMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlMPQ: boxMPQ.cmxa boxMPQ.a
mlD: boxD.cmxa boxD.a
mlMPFR: boxMPFR.cmxa boxMPFR.a
endif
ifneq ($(HAS_SHARED),)
mlMPQ: libboxMPQ_caml.so libboxMPQ_caml_debug.so dllboxMPQ_caml.so dllboxMPQ_caml_debug.so
mlD: libboxD_caml.so libboxD_caml_debug.so dllboxD_caml.so dllboxD_caml_debug.so
mlMPFR: libboxMPFR_caml.so libboxMPFR_caml_debug.so dllboxMPFR_caml.so dllboxMPFR_caml_debug.so
endif

mlexample%.byte: mlexample.ml box%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_INSTALL)/lib -I $(APRON_PREFIX)/lib -o $@ bigarray.cma gmp.cma apron.cma box$*.cma $<

mlexample%.opt: mlexample.ml box%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_INSTALL)/lib -I $(APRON_PREFIX)/lib -o $@ bigarray.cmxa gmp.cmxa apron.cmxa box$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.so *.annot *.cm[ioax] *.cmxa *.byte *.opt
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr boxpolkarung boxpolkatopg tmp

mostlyclean: clean
	/bin/rm -fr $(IDLDEP)
	/bin/rm -f box.ml box.mli box_caml.c

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALLd) $(PREFIX)/include $(PREFIX)/lib
	$(INSTALL) $(CCINC_TO_INSTALL) $(PREFIX)/include
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/lib; fi; \
	done
#	for i in $(CCBIN_TO_INSTALL); do \
#		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/bin; fi; \
#	done
	for i in $(CAML_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/lib; fi; \
	done

uninstall:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(PREFIX)/include/$$i; done
	/bin/rm -f $(PREFIX)/lib/libbox*.* $(PREFIX)/lib/liboct*_debug.*
	/bin/rm -f $(PREFIX)/lib/dllbox*.so $(PREFIX)/lib/dlloct*_debug.so
	/bin/rm -f $(PREFIX)/lib/box.mli $(PREFIX)/lib/box.ml $(PREFIX)/lib/box.cm[ix] $(PREFIX)/lib/box.idl $(PREFIX)/lib/box*.cma $(PREFIX)/lib/box*.cmxa $(PREFIX)/lib/box*.a

distclean: uninstall
	/bin/rm -f Makefile.depend

dist: $(CCSRC) Makefile sedscript_caml box.texi box.idl box.ml box.mli box_caml.c COPYING README
	(cd ..; tar zcvf box.tgz $(^:%=box/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

.PRECIOUS: libbox%.a libbox%_debug.a libbox%.so libbox%_debug.so
.PRECIOUS: libbox%_caml.a libbox%_caml_debug.a libbox%_caml.so libbox%_caml_debug.so
.PRECIOUS: %MPQ.o %D.o %MPFR.o
.PRECIOUS: %MPQ_debug.o %D_debug.o %MPFR_debug.o
.PRECIOUS: %.cmo %.cmx

#-----------------------------------
# C part
#-----------------------------------

libbox%.a: $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libbox%_debug.a: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libbox%.so:  $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LIBS)
libbox%_debug.so: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $^ $(LIBS_DEBUG)

%MPQ.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%MPQ_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%Rll.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%Rll_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%D.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%D_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%MPFR.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<
%MPFR_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<

#---------------------------------------
# OCaml binding
#---------------------------------------

libbox%_caml.a: box_caml.o libbox%.a
	$(AR) rcs $@ $<
	$(RANLIB) $@
libbox%_caml_debug.a: box_caml_debug.o libbox%_debug.a
	$(AR) rcs $@ $<
	$(RANLIB) $@

libbox%_caml.so: box_caml.o libbox%.so
	$(CC) $(CFLAGS) -shared -o $@ box_caml.o  $(LIBS) -L. -lbox$*
libbox%_caml_debug.so: box_caml_debug.o libbox%_debug.so
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ box_caml_debug.o $(LIBS_DEBUG) -L. -lbox$*_debug

dllbox%_caml.so: libbox%_caml.so
	ln -s -f $^ $@
dllbox%_caml_debug.so: libbox%_caml_debug.so
	ln -s -f $^ $@

box_caml.o: box_caml.c
	$(CC) $(CFLAGS) $(ICFLAGS) -I../apron -DNUM_MPQ -c -o $@ $<
box_caml_debug.o: box_caml.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -I../apron -DNUM_MPQ -c -o $@ $<

#---------------------------------------
# OCaml rules
#---------------------------------------

box%.cma: box.cmo libbox%_caml.a libbox%.a
	$(OCAMLMKLIB) -ocamlc "$(OCAMLC)" -verbose -o box$* -oc box$*_caml box.cmo $(LIBS) -L. -lbox$*

box%.cmxa box%.a: box.cmx libbox%_caml.a libbox%.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT)" -verbose -o box$* -oc box$*_caml box.cmx $(LIBS) -L. -lbox$* -L$(APRON_PREFIX)/lib

#---------------------------------------
# IDL rules
#---------------------------------------

$(IDLDEP): %.idl: ../mlapronidl/%.idl
	ln -s -f $< $@

# generates box.ml, box.mli, box_caml.c from box.idl
rebuild: $(IDLDEP) box.idl
	mkdir -p tmp
	cp box.idl tmp/box.idl
	$(CAMLIDL) -no-include -nocpp tmp/box.idl
	$(SED) -f ../mlapronidl/sedscript_c tmp/box_stubs.c >box_caml.c
	$(SED) -f sedscript_caml tmp/box.ml >box.ml
	$(SED) -f sedscript_caml tmp/box.mli >box.mli

#---------------------------------------
# ML generic rules
#---------------------------------------

%.cmi: %.mli
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmo: %.ml %.cmi
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c $<

%.cmx: %.ml %.cmi
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -c $<

#-----------------------------------
# DEPENDENCIES
#-----------------------------------
