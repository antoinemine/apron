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
-I../apron \
-I../num \
-I../itv \
-I../mlapronidl \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(CAMLIDL_PREFIX)/lib/ocaml -I$(CAML_PREFIX)/lib/ocaml

# Caml
OCAMLINC = -I $(MLGMPIDL_PREFIX)/lib -I ../mlapronidl

#---------------------------------------
# Files
#---------------------------------------

CCMODULES = \
mf_qsort \
pk_user pk_internal pk_bit pk_satmat pk_vector pk_matrix pk_cherni \
pk_representation pk_approximate pk_constructor pk_test pk_extract \
pk_meetjoin pk_assign pk_project pk_resize pk_expandfold \
pk_widening pk_closure \
pkeq

CCINC = \
pk_config.h pk.h pkeq.h \
mf_qsort.h pk_internal.h \
pk_user.h pk_bit.h pk_satmat.h pk_vector.h pk_matrix.h pk_cherni.h \
pk_representation.h pk_constructor.h pk_test.h pk_extract.h \
pk_meetjoin.h pk_assign.h pk_resize.h

CCSRC = $(CCINC) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = pk.h pkeq.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = \
libpolkaMPQ.a libpolkaMPQ_debug.a \
libpolkaRll.a libpolkaRll_debug.a \
libpolkaMPQ.so libpolkaMPQ_debug.so \
libpolkaRll.so libpolkaRll_debug.so \

CAML_TO_INSTALL = \
polka.idl polka.mli polka.ml polka.cmi \
polkaMPQ.cma \
polkaRll.cma \
libpolkaMPQ_caml.a libpolkaMPQ_caml_debug.a \
libpolkaRll_caml.a libpolkaRll_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAML_TO_INSTALL := $(CAML_TO_INSTALL) \
polkaMPQ.cmxa polkaMPQ.a \
polkaRll.cmxa polkaRll.a
endif
ifneq ($(HAS_SHARED),)
CAML_TO_INSTALL := $(CAML_TO_INSTALL) \
libpolkaMPQ_caml.so libpolkaMPQ_caml_debug.so \
libpolkaRll_caml.so libpolkaRll_caml_debug.so \
dllpolkaMPQ_caml.so dllpolkaMPQ_caml_debug.so \
dllpolkaRll_caml.so dllpolkaRll_caml_debug.so
endif

LIBS = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron
LIBS_DEBUG = -L$(GMP_PREFIX)/lib -lm -lgmp -L$(MPFR_PREFIX)/lib -lmpfr -L../apron -lapron_debug

#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allMPQ allRll

allMPQ: libpolkaMPQ.a libpolkaMPQ_debug.a
allRll: libpolkaRll.a libpolkaRll_debug.a
ifneq ($(HAS_SHARED),)
allMPQ: libpolkaMPQ.so libpolkaMPQ_debug.so
allRll: libpolkaRll.so libpolkaRll_debug.so
endif

ml: polka.mli polka.ml polka.cmi mlMPQ mlRll

mlMPQ: polkaMPQ.cma libpolkaMPQ_caml.a libpolkaMPQ_caml_debug.a
mlRll: polkaRll.cma libpolkaRll_caml.a libpolkaRll_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlMPQ: polkaMPQ.cmxa polkaMPQ.a
mlRll: polkaRll.cmxa polkaRll.a
endif
ifneq ($(HAS_SHARED),)
mlMPQ: libpolkaMPQ_caml.so libpolkaMPQ_caml_debug.so dllpolkaMPQ_caml.so dllpolkaMPQ_caml_debug.so
mlRll: libpolkaRll_caml.so libpolkaRll_caml_debug.so dllpolkaRll_caml.so dllpolkaRll_caml_debug.so
endif

test0%: test0%_debug.o libpolka%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ $< \
	-L. -lpolka$*_debug -L$(APRON_PREFIX)/lib -lapron_debug -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp -lm

test1%: test1%_debug.o libpolka%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ $< \
	-L. -lpolka$*_debug -L$(APRON_PREFIX)/lib -lapron_debug -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp -lm

mlexample%.byte: mlexample.ml box%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_INSTALL)/lib -I $(APRON_PREFIX)/lib -o $@ bigarray.cma gmp.cma apron.cma box$*.cma $<

mlexample%.opt: mlexample.ml box%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_INSTALL)/lib -I $(APRON_PREFIX)/lib -o $@ bigarray.cmxa gmp.cmxa apron.cmxa box$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.so
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -f test[01]Il* test[01]MPQ test[01]Il*_debug test[01]MPQ_debug
	/bin/rm -fr *.annot *.cm[ioax] *.cmxa
	/bin/rm -f manager.idl
	/bin/rm -fr tmp

mostlyclean: clean
	/bin/rm -f manager.idl
	/bin/rm -fr polka_caml.* polka.ml polka.mli

install:
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
	/bin/rm -f $(PREFIX)/lib/libpolka*.* $(PREFIX)/lib/liboct*_debug.*
	/bin/rm -f $(PREFIX)/lib/dllpolka*.so $(PREFIX)/lib/dlloct*_debug.so
	/bin/rm -f $(PREFIX)/lib/polka.mli $(PREFIX)/lib/polka.ml $(PREFIX)/lib/polka.cm[ix] $(PREFIX)/lib/polka.idl $(PREFIX)/lib/polka*.cma $(PREFIX)/lib/polka*.cmxa $(PREFIX)/lib/polka*.a

distclean: uninstall
	/bin/rm -f Makefile.depend

dist: $(CCSRC) Makefile sedscript_caml newpolka.texi polka.idl polka.ml polka.mli polka_caml.c COPYING README
	(cd ..; tar zcvf newpolka.tgz $(^:%=newpolka/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

.PRECIOUS: libpolka%.a libpolka%_debug.a libpolka%.so libpolka%_debug.so 
.PRECIOUS: libpolka%_caml.a libpolka%_caml_debug.a libpolka%_caml.so libpolka%_caml_debug.so 
.PRECIOUS: %Rl.o %Rll.o %MPQ.o
.PRECIOUS: %Rl_debug.o %Rll_debug.o %MPQ_debug.o
.PRECIOUS: %.cmo %.cmx

#-----------------------------------
# C part
#-----------------------------------

cxref: $(CCMODULES:%=%.c)
	cxref -I/usr/include -I/usr/lib/gcc/i386-redhat-linux/4.0.2/include $(ICFLAGS) -DNUM_MPQ -no-comments -xref-func -index-func -html32-src -Otmp $^

libpolka%.a: $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libpolka%_debug.a: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libpolka%.so:  $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LIBS)
libpolka%_debug.so: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $^ $(LIBS_DEBUG)

%Rl.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rl_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rll.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%Rll_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%MPQ.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -I$(GMP_PREFIX)/include -c -o $@ $<
%MPQ_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -I$(GMP_PREFIX)/include -c -o $@ $<

#-----------------------------------
# Caml part
#-----------------------------------

test_caml: test.cmo polkarun
	$(OCAMLC) -g $(OCAMLINC) -o $@ -use-runtime polkarun \
	bigarray.cma gmp.cma apron.cma polkaMPQ.cma test.cmo

test.cmo: test.ml
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -I $(CAMLLIB_PREFIX)/lib -c $<

#---------------------------------------
# OCaml binding
#---------------------------------------

libpolka%_caml.a: polka_caml.o libpolka%.a
	$(AR) rcs $@ $<
	$(RANLIB) $@
libpolka%_caml_debug.a: polka_caml_debug.o libpolka%_debug.a
	$(AR) rcs $@ $<
	$(RANLIB) $@

libpolka%_caml.so: polka_caml.o libpolka%.so
	$(CC) $(CFLAGS) -shared -o $@ polka_caml.o $(LIBS) -L. -lpolka$*
libpolka%_caml_debug.so: polka_caml_debug.o libpolka%_debug.so
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ polka_caml_debug.o $(LIBS_DEBUG) -L. -lpolka$*_debug

dllpolka%_caml.so: libpolka%_caml.so
	ln -s -f $^ $@
dllpolka%_caml_debug.so: libpolka%_caml_debug.so
	ln -s -f $^ $@

#---------------------------------------
# OCaml rules
#---------------------------------------

polka%.cma: polka.cmo libpolka%_caml.a libpolka%.a
	$(OCAMLMKLIB) -ocamlc "$(OCAMLC)" -verbose -o polka$* -oc polka$*_caml polka.cmo $(LIBS) -L. -lpolka$*

polka%.cmxa polka%.a: polka.cmx libpolka%_caml.a libpolka%.a
	$(OCAMLMKLIB) -ocamlopt "$(OCAMLOPT)" -verbose -o polka$* -oc polka$*_caml polka.cmx $(LIBS) -L. -lpolka$*

#---------------------------------------
# IDL rules
#---------------------------------------

manager.idl: ../mlapronidl/manager.idl
	ln -s -f $< $@

# generates X.ml, X.mli, X_caml.c for X = polka
rebuild: manager.idl polka.idl
	mkdir -p tmp
	for i in polka; do \
		echo "module $$i"; \
		cp $${i}.idl tmp/$${i}.idl; \
		$(CAMLIDL) -no-include -nocpp tmp/$${i}.idl; \
		cp tmp/$${i}_stubs.c $${i}_caml.c; \
		$(SED) -f sedscript_caml tmp/$${i}.ml >$${i}.ml; \
		$(SED) -f sedscript_caml tmp/$${i}.mli >$${i}.mli; \
	done

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUMINT_MPQ -c -o $@ $<
%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUMINT_MPQ -c -o $@ $<

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
