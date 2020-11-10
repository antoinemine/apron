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
-I../apron \
-I../num \
-I../itv \
-I../mlgmpidl \
-I../mlapronidl \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(CAMLIDL_PREFIX)/lib/ocaml -I$(CAML_PREFIX)/lib/ocaml

# Caml
OCAMLINC = -I ../mlgmpidl -I ../mlapronidl

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

CCSRC = pk_config.h pk.h pkeq.h \
mf_qsort.h pk_internal.h \
pk_user.h pk_bit.h pk_satmat.h pk_vector.h pk_matrix.h pk_cherni.h \
pk_representation.h pk_constructor.h pk_test.h pk_extract.h \
pk_meetjoin.h pk_assign.h pk_resize.h \
$(CCMODULES:%=%.c)

CCINC_TO_INSTALL = pk.h pkeq.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = libpolka.a libpolka_debug.a libpolkaMPQ.a libpolkaMPQ_debug.a libpolka_caml.a libpolka_caml_debug.a

CAML_TO_INSTALL = polka.idl \
polka.cma polka.cmxa polka.a \
polka.cmi polka.cmx polka.mli polka.ml

#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allMPQ libpolka.a libpolka_debug.a 

ml: $(CAML_TO_INSTALL) libpolka_caml.a libpolka_caml_debug.a

allMPQ: libpolkaMPQ.a libpolkaMPQ_debug.a

test0: test0MPQ_debug.o libpolka_debug.a
	$(CC) $(ICFLAGS) $(XCFLAGS) -I$(GMP_PREFIX)/include -o $@ $< \
	-L. -lpolka_debug -L$(APRON_PREFIX)/lib -lapron_debug -lgmp -lm

test1: test1MPQ_debug.o libpolka_debug.a
	$(CC) $(ICFLAGS) $(XCFLAGS) -I$(GMP_PREFIX)/include -o $@ $< \
	-L. -lpolka_debug -L$(APRON_PREFIX)/lib -lapron_debug -lgmp -lm

clean:
	/bin/rm -f *.[ao]
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -f test[01]Il* test[01]MPQ test[01]Il*_debug test[01]MPQ_debug
	/bin/rm -fr *.cm[ioax] *.cmxa
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

distclean:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(PREFIX)/include/$$i; done
	for i in $(CCLIB_TO_INSTALL); do /bin/rm -f $(PREFIX)/lib/$$i; done
#	for i in $(CCBIN_TO_INSTALL); do /bin/rm -f $(PREFIX)/bin/$$i; done
	for i in $(CAML_TO_INSTALL); do /bin/rm -f $(PREFIX)/lib/$$i; done
	/bin/rm -f Makefile.depend

dist: $(CCSRC) Makefile sedscript_caml newpolka.texi polka.idl polka.ml polka.mli polka_caml.c COPYING README
	(cd ..; tar zcvf newpolka.tgz $(^:%=newpolka/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

#-----------------------------------
# C part
#-----------------------------------

cxref: $(CCMODULES:%=%.c)
	cxref -I/usr/include -I/usr/lib/gcc/i386-redhat-linux/4.0.2/include $(ICFLAGS) -DNUM_MPQ -no-comments -xref-func -index-func -html32-src -Otmp $^

libpolka.a: libpolkaMPQ.a
	cp $^ $@
libpolka_debug.a: libpolkaMPQ_debug.a
	cp $^ $@

libpolkaMPQ.a: $(CCMODULES:%=%MPQ.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libpolkaMPQ_debug.a: $(CCMODULES:%=%MPQ_debug.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

%Rl.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rll.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%MPQ.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -I$(GMP_PREFIX)/include -c -o $@ $<

%MPQ_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -I$(GMP_PREFIX)/include -c -o $@ $<

#-----------------------------------
# Caml part
#-----------------------------------

polkatop: polka.cma libpolka_caml.a libpolka_debug.a
	$(OCAMLMKTOP) $(OCAMLINC) $(OCAMLFLAGS) -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma polka.cma \
	-ccopt "-L. -L../mlgmpidl -L../mlapronidl -L../apron" \
	-cclib "-lpolka_debug"

polkarun: polka.cma libpolka_caml_debug.a libpolka_debug.a
	$(OCAMLC) $(OCAMLINC) $(OCAMLFLAGS) -verbose -o $@ \
	-make-runtime bigarray.cma gmp.cma apron.cma polka.cma \
	-ccopt "-L. -L../mlgmpidl -L../mlapronidl -L../apron" \
	-cclib "-lpolka_debug"

test_caml: test.cmo polkarun
	$(OCAMLC) -g $(OCAMLINC) -o $@ -use-runtime polkarun \
	bigarray.cma gmp.cma apron.cma polka.cma test.cmo

test.cmo: test.ml
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -I $(CAMLLIB_PREFIX)/lib -c $<

#---------------------------------------
# C rules
#---------------------------------------


libpolka_caml.a: polka_caml.o 
	$(AR) rcs $@ $^
	$(RANLIB) $@

libpolka_caml_debug.a: polka_caml_debug.o 
	$(AR) rcs $@ $^
	$(RANLIB) $@

#---------------------------------------
# ML rules
#---------------------------------------

polka.cma: polka.cmo libpolka_caml.a libpolka.a
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -custom -a -o $@ \
	polka.cmo \
	-ccopt "-L$(APRON_PREFIX)/lib" -cclib "-lpolka_caml -lpolka"

polka.cmxa: polka.cmx libpolka_caml.a libpolka.a
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -a -o $@ \
	polka.cmx \
	-ccopt "-L$(APRON_PREFIX)/lib" -cclib "-lpolka_caml -lpolka"
	$(RANLIB) polka.a

#---------------------------------------
# IDL rules
#---------------------------------------

manager.idl: ../mlapronidl/manager.idl
	ln -s $< $@

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
