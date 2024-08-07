include ../Makefile.config
include ../vars.mk

ifneq ($(CSDP_PATH),)
CSDP_CFLAGS = -ansi -Wall -DNOSHORTS -DUSEGETTIME -I$(CSDP_PATH)/include -D_USE_SDP
CSDP_LIBS = -L/usr/lib/sse2 -L$(CSDP_PATH)/lib -lsdp -llapack -lf77blas -lcblas -latlas -lgfortran -lm
endif

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

CFLAGS := $(CSDP_CFLAGS) $(CFLAGS)
CFLAGS_DEBUG := $(CSDP_CFLAGS) $(CFLAGS_DEBUG) -D_GET_CPU_TIME -D_T1P_DEBUG #-D_T1P_DEBUG_FUN #-pg -D_CALL_DEBUG

ICFLAGS += $(BASE_ICFLAGS) $(ML_ICFLAGS)
LDFLAGS += $(BASE_LIFLAGS) -L../box -L../newpolka
CMXSINC = $(APRON_CMXSINC) -I ../box -I ../newpolka -I .

#---------------------------------------
# Files
#---------------------------------------

CCMODULES = t1p_internal t1p_representation t1p_constructor t1p_meetjoin t1p_assign t1p_resize t1p_otherops t1p_fun t1p_itv_utils
CCSRC = t1p.h t1p_macro_def.h t1p_test.c t1p_test_eval_texp.c $(CCMODULES:%=%.h) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = t1p.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = libt1pMPQ.a libt1pD.a libt1pMPFR.a
CCLIB_TO_INSTALL_DEBUG = libt1pMPQ_debug.a libt1pD_debug.a libt1pMPFR_debug.a
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL := $(CCLIB_TO_INSTALL) \
libt1pMPQ.$(EXT_DLL) libt1pD.$(EXT_DLL) libt1pMPFR.$(EXT_DLL)
CCLIB_TO_INSTALL_DEBUG := $(CCLIB_TO_INSTALL_DEBUG) \
libt1pMPQ_debug.$(EXT_DLL) libt1pD_debug.$(EXT_DLL) libt1pMPFR_debug.$(EXT_DLL)
endif

ifneq ($(HAS_OCAML),)
CAML_TO_INSTALL = \
t1p.idl t1p.ml t1p.mli t1p.cmi \
t1pMPQ.cma \
t1pD.cma \
t1pMPFR.cma \
libt1pMPQ_caml.a libt1pD_caml.a libt1pMPFR_caml.a
CAML_TO_INSTALL_DEBUG = \
libt1pMPQ_caml_debug.a libt1pD_caml_debug.a libt1pMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
  CAML_TO_INSTALL += $(call OCAMLOPT_TARGETS, t1pMPQ t1pD t1pMPFR)
  CAML_TO_INSTALL_DEBUG += $(call OCAMLOPT_TARGETS, t1pMPQ.d t1pD.d t1pMPFR.d)
endif
ifneq ($(HAS_SHARED),)
CAML_TO_INSTALL += dllt1pMPQ_caml.$(EXT_DLL) dllt1pD_caml.$(EXT_DLL) dllt1pMPFR_caml.$(EXT_DLL)
CAML_TO_INSTALL_DEBUG += dllt1pMPQ_caml_debug.$(EXT_DLL) dllt1pD_caml_debug.$(EXT_DLL) dllt1pMPFR_caml_debug.$(EXT_DLL)
endif
endif

LIBS = -lapron -lgmpxx -lmpfr -lgmp -lm
LIBS_DEBUG = -lapron_debug -lgmpxx -lmpfr -lgmp -lm

#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allMPQ allD allMPFR 
# testD testMPQ testMPFR testD_debug testMPQ_debug testMPFR_debug test_joinD_debug test_joinMPQ_debug test_joinMPFR_debug 


allMPQ: libt1pMPQ.a libt1pMPQ_debug.a
allD: libt1pD.a libt1pD_debug.a
allMPFR: libt1pMPFR.a libt1pMPFR_debug.a
ifneq ($(HAS_SHARED),)
allMPQ: libt1pMPQ.$(EXT_DLL) libt1pMPQ_debug.$(EXT_DLL)
allD: libt1pD.$(EXT_DLL) libt1pD_debug.$(EXT_DLL)
allMPFR: libt1pMPFR.$(EXT_DLL) libt1pMPFR_debug.$(EXT_DLL)
endif

exemple1%_debug: exemple1%_debug.o
	$(CC) $(CFLAGS_DEBUG) -DNUM_DOUBLE -o $@ -L. $< -lt1p$*_debug -lbox$*_debug -loct$*_debug $(LDFLAGS) $(LIBS_DEBUG)

#test%: t1p_test_eval_texp%.o
#	$(CC) $(CFLAGS) -DNUM_DOUBLE -o $@ $(LIBS) -L. $< -lt1p$* -lbox$* -lpolkaMPQ_debug -lapron t1p_internal$*.o t1p_representation$*.o

#test%_debug: t1p_test_eval_texp%_debug.o
#	$(CC) $(CFLAGS_DEBUG) -DNUM_DOUBLE -o $@ -L. $< -lt1p$*_debug -lbox$*_debug -lpolkaMPQ_debug t1p_internal$*_debug.o $(LIBS_DEBUG)


#test_join%: t1p_test_join%.o
#	$(CC) $(CFLAGS_DEBUG) -DNUM_DOUBLE -o $@ $(LIBS) -L. $< -lt1p$* -lbox$* -lpolkaMPQ_debug t1p_internal$*.o t1p_representation$*.o

#test_join%_debug: t1p_test_join%_debug.o
#	$(CC) $(CFLAGS_DEBUG) -DNUM_DOUBLE -o $@ -L. $< -lt1p$*_debug -lbox$*_debug -lpolkaMPQ_debug t1p_internal$*_debug.o $(LIBS_DEBUG)


ml: t1p.mli t1p.ml t1p.cmi mlMPQ mlD mlMPFR

mlMPQ: t1pMPQ.cma libt1pMPQ_caml.a libt1pMPQ_caml_debug.a 
mlD: t1pD.cma libt1pD_caml.a libt1pD_caml_debug.a
mlMPFR: t1pMPFR.cma libt1pMPFR_caml.a libt1pMPFR_caml_debug.a
ifneq ($(HAS_OCAMLOPT),)
mlMPQ: $(call OCAMLOPT_TARGETS, t1pMPQ t1pMPQ.d)
mlD: $(call OCAMLOPT_TARGETS, t1pD t1pD.d)
mlMPFR: $(call OCAMLOPT_TARGETS, t1pMPFR t1pMPFR.d)
endif
ifneq ($(HAS_SHARED),)
mlMPQ: dllt1pMPQ_caml.$(EXT_DLL) dllt1pMPQ_caml_debug.$(EXT_DLL)
mlD: dllt1pD_caml.$(EXT_DLL)  dllt1pD_caml_debug.$(EXT_DLL) 
mlMPFR: dllt1pMPFR_caml.$(EXT_DLL) dllt1pMPFR_caml_debug.$(EXT_DLL)
endif

mlexample%.byte: mlexample.ml t1p%.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma t1p$*.cma $<

mlexample%.opt: mlexample.ml t1p%.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa t1p$*.cmxa $<

clean:
	/bin/rm -f *.[ao] *.$(EXT_DLL) *.cm[ioax] *.cmx[as] *.byte *.opt *.annot
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr t1ppolkarung t1ppolkatopg tmp
	/bin/rm -fr test*
	/bin/rm -f t1p.ml t1p.mli t1p_caml.c
	/bin/rm -f Makefile.depend

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
	/bin/rm -f $(APRON_LIB)/libt1p*.* $(APRON_LIB)/libt1p*_debug.*
	/bin/rm -f $(APRON_LIB)/dllt1p*.$(EXT_DLL) $(APRON_LIB)/dllt1p*_debug.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/t1p.mli $(APRON_LIB)/t1p.ml $(APRON_LIB)/t1p.cm[ix] $(APRON_LIB)/t1p.idl $(APRON_LIB)/t1p*.cma $(APRON_LIB)/t1p*.cmx[as] $(APRON_LIB)/t1p*.a


dist: $(CCSRC) Makefile perlscript_caml.pl t1p.idl t1p.ml t1p.mli t1p_caml.c
	(cd ..; tar zcvf taylor1plus.tgz $(^:%=taylor1plus/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

.PRECIOUS: libt1p%.a libt1p%_debug.a libt1p%.$(EXT_DLL) libt1p%_debug.$(EXT_DLL)
.PRECIOUS: libt1p%_caml.a libt1p%_caml_debug.a dllt1p%_caml.$(EXT_DLL)
.PRECIOUS: %MPQ.o %D.o %MPFR.o
.PRECIOUS: %MPQ_debug.o %D_debug.o %MPFR_debug.o
.PRECIOUS: %.cmo %.cmx

#-----------------------------------
# C part
#-----------------------------------

libt1p%.a: $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libt1p%_debug.a: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
##
## NOTE: $(LIBS) and $(LIBS_DEBUG) are pushed to the end of the two following rules as 
## mingw-w64 linker is not very smart and is sensitive to the library order
## should work across all archs
##
libt1p%.$(EXT_DLL):  $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lbox$* -lpolkaMPQ $(LIBS)
libt1p%_debug.$(EXT_DLL): $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(CC_APRON_DYLIB) $(CFLAGS_DEBUG) -o $@ $^ $(LDFLAGS) -lbox$*_debug -lpolkaMPQ_debug $(LIBS_DEBUG)

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

dllt1p%_caml.$(EXT_DLL) libt1p%_caml.a: t1p_caml.o libt1p%.a
	$(OCAMLMKLIB) -o t1p$*_caml t1p_caml.o -L. -lt1p$* $(LDFLAGS) $(LIBS)

dllt1p%_caml_debug.$(EXT_DLL) libt1p%_caml_debug.a: t1p_caml_debug.o libt1p%_debug.a
	$(OCAMLMKLIB) -o t1p$*_caml_debug t1p_caml_debug.o -L. -lt1p$*_debug $(LDFLAGS) $(LIBS_DEBUG)

t1p_caml.o: t1p_caml.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<

t1p_caml_debug.o: t1p_caml.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<

#---------------------------------------
# OCaml rules
#---------------------------------------

t1p%.cma: t1p.cmo libt1p%_caml.a libt1p%.a
	$(OCAMLMKLIB) -o t1p$* -oc t1p$*_caml t1p.cmo -lt1p$* $(LIBS)

t1p%.cmxa t1p%.a: t1p.cmx libt1p%_caml.a libt1p%.a
	$(OCAMLMKLIB) -o t1p$* -oc t1p$*_caml t1p.cmx -lt1p$* $(LIBS)

t1p%.d.cmxa t1p%.d.a: t1p.cmx libt1p%_caml_debug.a libt1p%_debug.a
	$(OCAMLMKLIB) -o t1p$*.d -oc t1p$*_caml_debug t1p.cmx -lt1p$*_debug $(LIBS_DEBUG)

#---------------------------------------
# IDL rules
#---------------------------------------

# generates t1p.ml, t1p.mli, t1p_caml.c from t1p.idl
%.ml %.mli %_caml.c: %.idl perlscript_caml.pl ../mlapronidl/manager.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/manager.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -nocpp $*.idl
	cp tmp/$*_stubs.c $*_caml.c
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
