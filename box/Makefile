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

CCMODULES = box_internal box_representation box_constructor	\
box_meetjoin box_assign box_resize box_otherops box_policy
CCSRC = box_config.h box.h $(CCMODULES:%=%.h) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = box.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = libboxMPQ.a libboxD.a libboxMPFR.a
CCLIB_TO_INSTALL_DEBUG = libboxMPQ_debug.a libboxD_debug.a libboxMPFR_debug.a
ifneq ($(HAS_SHARED),)
  CCLIB_TO_INSTALL := $(CCLIB_TO_INSTALL) libboxMPQ.$(EXT_DLL) libboxD.$(EXT_DLL) libboxMPFR.$(EXT_DLL)
  CCLIB_TO_INSTALL_DEBUG := $(CCLIB_TO_INSTALL_DEBUG) libboxMPQ_debug.$(EXT_DLL) libboxD_debug.$(EXT_DLL) libboxMPFR_debug.$(EXT_DLL)
endif

CAML_TO_INSTALL = box.idl box.ml box.mli box.cmi boxMPQ.cma	\
boxD.cma boxMPFR.cma libboxMPQ_caml.a libboxD_caml.a		\
libboxMPFR_caml.a
CAML_TO_INSTALL_DEBUG = libboxMPQ_caml_debug.a libboxD_caml_debug.a	\
libboxMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
  CAML_TO_INSTALL += $(call OCAMLOPT_TARGETS, boxMPQ boxD boxMPFR)
  CAML_TO_INSTALL_DEBUG += $(call OCAMLOPT_TARGETS, boxMPQ.d boxD.d boxMPFR.d)
endif
ifneq ($(HAS_SHARED),)
  CAML_TO_INSTALL += dllboxMPQ_caml.$(EXT_DLL) dllboxD_caml.$(EXT_DLL)	\
  dllboxMPFR_caml.$(EXT_DLL)
  CAML_TO_INSTALL_DEBUG += dllboxMPQ_caml_debug.$(EXT_DLL) dllboxD_caml_debug.$(EXT_DLL)	\
  dllboxMPFR_caml_debug.$(EXT_DLL)
endif


LIBS = -lapron -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lmpfr -lgmp -lm

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
  allMPQ: libboxMPQ.$(EXT_DLL) libboxMPQ_debug.$(EXT_DLL)
  allD: libboxD.$(EXT_DLL) libboxD_debug.$(EXT_DLL)
  allMPFR: libboxMPFR.$(EXT_DLL) libboxMPFR_debug.$(EXT_DLL)
endif

ml: box.mli box.ml box.cmi mlMPQ mlD mlMPFR

mlMPQ: boxMPQ.cma libboxMPQ_caml.a libboxMPQ_caml_debug.a 
mlD: boxD.cma libboxD_caml.a libboxD_caml_debug.a
mlMPFR: boxMPFR.cma libboxMPFR_caml.a libboxMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
  mlMPQ: $(call OCAMLOPT_TARGETS, boxMPQ boxMPQ.d)
  mlD: $(call OCAMLOPT_TARGETS, boxD boxD.d)
  mlMPFR: $(call OCAMLOPT_TARGETS, boxMPFR boxMPFR.d)
endif
ifneq ($(HAS_SHARED),)
  mlMPQ: dllboxMPQ_caml.$(EXT_DLL) 
  mlD: dllboxD_caml.$(EXT_DLL)
  mlMPFR: dllboxMPFR_caml.$(EXT_DLL)
endif

mlexample%.byte: mlexample.ml box%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma box$*.cma $<

mlexample%.opt: mlexample.ml box%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa box$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) *.annot *.cm[ioax] *.cmx[as] *.byte *.opt
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr boxpolkarung boxpolkatopg tmp
	/bin/rm -f box.ml box.mli box_caml.c
	/bin/rm -f Makefile.depend

mostlyclean: clean
distclean: clean

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
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
	/bin/rm -f $(APRON_LIB)/libbox*.* $(APRON_LIB)/liboct*_debug.*
	/bin/rm -f $(APRON_LIB)/dllbox*.$(EXT_DLL) $(APRON_LIB)/dlloct*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/box.mli $(APRON_LIB)/box.ml		\
	           $(APRON_LIB)/box.cm[ix] $(APRON_LIB)/box.idl		\
	           $(APRON_LIB)/box*.cma $(APRON_LIB)/box*.cmx[as]	\
	           $(APRON_LIB)/box*.a


dist: $(CCSRC) Makefile perlscript_caml.pl box.texi box.idl box.ml box.mli box_caml.c COPYING README
	(cd ..; tar zcvf box.tgz $(^:%=box/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

.PRECIOUS: libbox%.a libbox%_debug.a libbox%.$(EXT_DLL) libbox%_debug.$(EXT_DLL)
.PRECIOUS: libbox%_caml.a libbox%_caml_debug.a dllbox%_caml.$(EXT_DLL)
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
libbox%.$(EXT_DLL):  $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
libbox%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS_DEBUG) -o $@ $^ $(LDFLAGS) $(LIBS_DEBUG)

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

dllbox%_caml.$(EXT_DLL) libbox%_caml.a: box_caml.o libbox%.a
	$(OCAMLMKLIB) -o box$*_caml $< -L. -lbox$* $(LDFLAGS) $(LIBS)

dllbox%_caml_debug.$(EXT_DLL) libbox%_caml_debug.a: box_caml_debug.o libbox%_debug.a
	$(OCAMLMKLIB) -o box$*_caml_debug $< -L. -lbox$*_debug $(LDFLAGS) $(LIBS_DEBUG)

box_caml.o: box_caml.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
box_caml_debug.o: box_caml.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<

#---------------------------------------
# OCaml rules
#---------------------------------------

box%.cma: box.cmo libbox%_caml.a libbox%.a
	$(OCAMLMKLIB) -o box$* -oc box$*_caml box.cmo -lbox$* $(LIBS)

box%.cmxa box%.a: box.cmx libbox%_caml.a libbox%.a
	$(OCAMLMKLIB) -o box$* -oc box$*_caml box.cmx -lbox$* $(LIBS)

box%.d.cmxa box%.d.a: box.cmx libbox%_caml_debug.a libbox%_debug.a
	$(OCAMLMKLIB) -o box$*.d -oc box$*_caml_debug box.cmx -lbox$*_debug $(LIBS_DEBUG)

#---------------------------------------
# IDL rules
#---------------------------------------

%.ml %.mli %_caml.c: %.idl ../mlapronidl/perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp -I . $*.idl
	$(PERL) ../mlapronidl/perlscript_c.pl < tmp/$*_stubs.c >$*_caml.c
	$(PERL) perlscript_caml.pl < tmp/$*.ml >$*.ml
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
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC) -c $<

%.cmxs: %.cmxa
	$(OCAMLOPT_CMXS) $(CMXSINC) -o $@ $<

#-----------------------------------
# DEPENDENCIES
#-----------------------------------
