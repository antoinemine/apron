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

CCMODULES = \
pk_user pk_internal pk_bit pk_satmat pk_vector pk_matrix pk_cherni \
pk_representation pk_approximate pk_constructor pk_test pk_extract \
pk_meetjoin pk_assign pk_project pk_resize pk_expandfold \
pk_widening pk_closure \
pkeq

CCINC = \
pk_config.h pk.h pkeq.h \
pk_qsort.h pk_internal.h \
pk_user.h pk_bit.h pk_satmat.h pk_vector.h pk_matrix.h pk_cherni.h \
pk_representation.h pk_constructor.h pk_test.h pk_extract.h \
pk_meetjoin.h pk_assign.h pk_resize.h

CCSRC = $(CCINC) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = pk.h pkeq.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = libpolkaMPQ.a libpolkaRll.a
CCLIB_TO_INSTALL_DEBUG = libpolkaMPQ_debug.a libpolkaRll_debug.a
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libpolkaMPQ.$(EXT_DLL) libpolkaRll.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG += libpolkaMPQ_debug.$(EXT_DLL) libpolkaRll_debug.$(EXT_DLL)
endif

CAML_TO_INSTALL = \
polka.idl polka.mli polka.ml polka.cmi \
polkaMPQ.cma \
polkaRll.cma \
libpolkaMPQ_caml.a \
libpolkaRll_caml.a
CAML_TO_INSTALL_DEBUG = \
libpolkaMPQ_caml_debug.a \
libpolkaRll_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
CAML_TO_INSTALL += $(call OCAMLOPT_TARGETS, $(addprefix polka,MPQ Rll))
CAML_TO_INSTALL_DEBUG += $(call OCAMLOPT_TARGETS, $(addprefix polka,MPQ.d Rll.d))
endif
ifneq ($(HAS_SHARED),)
CAML_TO_INSTALL += dllpolkaMPQ_caml.$(EXT_DLL) dllpolkaRll_caml.$(EXT_DLL)
endif

LIBS = -lapron -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lm

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
allMPQ: libpolkaMPQ.$(EXT_DLL) libpolkaMPQ_debug.$(EXT_DLL)
allRll: libpolkaRll.$(EXT_DLL) libpolkaRll_debug.$(EXT_DLL)
endif

ml: polka.mli polka.ml polka.cmi mlMPQ mlRll

mlMPQ: polkaMPQ.cma libpolkaMPQ_caml.a libpolkaMPQ_caml_debug.a
mlRll: polkaRll.cma libpolkaRll_caml.a libpolkaRll_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlMPQ: $(call OCAMLOPT_TARGETS, polkaMPQ polkaMPQ.d)
mlRll: $(call OCAMLOPT_TARGETS, polkaRll polkaRll.d)
endif
ifneq ($(HAS_SHARED),)
mlMPQ: dllpolkaMPQ_caml.$(EXT_DLL)
mlRll: dllpolkaRll_caml.$(EXT_DLL)
endif

test0%: test0%_debug.o libpolka%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ $< -L. -lpolka$*_debug $(LDFLAGS) $(LIBS_DEBUG)

test1%: test1%_debug.o libpolka%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ $< -L. -lpolka$*_debug $(LDFLAGS) $(LIBS_DEBUG)

mlexample%.byte: mlexample.ml box%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma box$*.cma $<

mlexample%.opt: mlexample.ml box%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa box$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL)
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -f test[01]Il* test[01]MPQ test[01]Il*_debug test[01]MPQ_debug
	/bin/rm -fr *.annot *.cm[ioax] *.cmx[as]
	/bin/rm -fr tmp
	/bin/rm -fr polka_caml.* polka.ml polka.mli
	/bin/rm -f Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_LIB)
	$(INSTALL) $(CCINC_TO_INSTALL) $(APRON_INCLUDE)
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i; fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAML_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
ifneq ($(HAS_DEBUG),)
	for i in $(CCLIB_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAML_TO_INSTALL_DEBUG); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif
endif

uninstall:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(APRON_INCLUDE)/$$i; done
	/bin/rm -f $(APRON_LIB)/libpolka*.* $(APRON_LIB)/libpolka*_debug.*
	/bin/rm -f $(APRON_LIB)/dllpolka*.$(EXT_DLL) $(APRON_LIB)/dllpolka*_debug.$(EXT_DLL)
ifeq ($(OCAMLFIND),)
	/bin/rm -f $(APRON_LIB)/polka.mli $(APRON_LIB)/polka.ml $(APRON_LIB)/polka.cm[ix] $(APRON_LIB)/polka.idl $(APRON_LIB)/polka*.cma $(APRON_LIB)/polka*.cmx[as] $(APRON_LIB)/polka*.a
endif


dist: $(CCSRC) Makefile perlscript_caml.pl newpolka.texi polka.idl polka.ml polka.mli polka_caml.c COPYING README
	(cd ..; tar zcvf newpolka.tgz $(^:%=newpolka/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

.PRECIOUS: libpolka%.a libpolka%_debug.a libpolka%.$(EXT_DLL) libpolka%_debug.$(EXT_DLL)
.PRECIOUS: libpolka%_caml.a libpolka%_caml_debug.a dllpolka%_caml.$(EXT_DLL)
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
libpolka%.$(EXT_DLL):  $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
libpolka%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS_DEBUG) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

%Rl.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rl_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rll.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%Rll_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%MPQ.o: %.c $(CCINC)
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%MPQ_debug.o: %.c $(CCINC)
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<

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

dllpolka%_caml.$(EXT_DLL) libpolka%_caml.a: polka_caml.o libpolka%.a
	$(OCAMLMKLIB) -o polka$*_caml $< -L. -lpolka$* $(LDFLAGS) $(LIBS)

libpolka%_caml_debug.a: polka_caml_debug.o libpolka%_debug.a
	$(OCAMLMKLIB) -o polka$*_caml_debug $< -L. -lpolka$*_debug $(LDFLAGS) $(LIBS_DEBUG)

#---------------------------------------
# OCaml rules
#---------------------------------------

polka%.cma: polka.cmo libpolka%_caml.a libpolka%.a
	$(OCAMLMKLIB) -o polka$* -oc polka$*_caml polka.cmo -lpolka$* $(LIBS)

polka%.cmxa polka%.a: polka.cmx libpolka%_caml.a libpolka%.a
	$(OCAMLMKLIB) -o polka$* -oc polka$*_caml polka.cmx -lpolka$* $(LIBS)

polka%.d.cmxa polka%.d.a: polka.cmx libpolka%_caml_debug.a libpolka%_debug.a
	$(OCAMLMKLIB) -o polka$*.d -oc polka$*_caml_debug polka.cmx -lpolka$*_debug $(LIBS_DEBUG)

#---------------------------------------
# IDL rules
#---------------------------------------

%.ml %.mli %_caml.c: %.idl perlscript_caml.pl ../mlapronidl/manager.idl
	mkdir -p tmp
	cp ../mlapronidl/manager.idl $*.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp $*.idl
	cp tmp/$*_stubs.c $*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
	$(PERL) perlscript_caml.pl < tmp/$*.mli >$*.mli

rebuild:
	@echo "make rebuild is no longer necessary"

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

%.cmxs: %.cmxa
	$(OCAMLOPT_CMXS) $(CMXSINC) -o $@ $<

#-----------------------------------
# DEPENDENCIES
#-----------------------------------
