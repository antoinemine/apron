include ../Makefile.config
include ../vars.mk

#---------------------------------------
# Programs
#---------------------------------------

# NB: Not sure this is required...
OCAMLMKLIB := $(OCAMLMKLIB) -dllpath $(PPL_PREFIX)/lib

#---------------------------------------
# Flags
#---------------------------------------

ICFLAGS += $(BASE_ICFLAGS)  $(ML_ICFLAGS) $(PPL_ICFLAGS) -I../ppl -I../newpolka
LDFLAGS += $(BASE_LIFLAGS) $(PPL_LIFLAGS) -L../ppl

# Caml
OCAMLINC += -I ../newpolka -I ../ppl
CMXSINC = $(APRON_CMXSINC) $(OCAMLINC) -I .

#---------------------------------------
# Files
#---------------------------------------

CCMODULES = ap_pkgrid 
CCSRC = $(CCMODULES:%=%.h) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = ap_pkgrid.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = libap_pkgrid.a libap_pkgrid_debug.a
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libap_pkgrid.so libap_pkgrid_debug.so
endif

ifneq ($(HAS_OCAML),)
  CAML_TO_INSTALL := polkaGrid.mli polkaGrid.cmi polkaGrid.cma	\
  libpolkaGrid_caml.a libpolkaGrid_caml_debug.a
  ifneq ($(HAS_OCAMLOPT),)
    CAML_TO_INSTALL += polkaGrid.cmxa polkaGrid.a
    ifneq ($(HAS_NATIVE_PLUGINS),)
      CAML_TO_INSTALL += polkaGrid.cmxs
    endif
  endif
  ifneq ($(HAS_SHARED),)
    CAML_TO_INSTALL += libap_pkgrid.so libap_pkgrid_debug.so	\
    dllpolkaGrid_caml.so
  endif
endif

LIBS = -lap_ppl -lppl -lapron -lgmpxx -lgmp -lmpfr -lm
LIBS_DEBUG = -lap_ppl_debug -lppl -lapron_debug -lgmpxx -lgmp -lmpfr -lm

#---------------------------------------
# Rules
#---------------------------------------

all: libap_pkgrid.a libap_pkgrid_debug.a 
ifneq ($(HAS_SHARED),)
all: libap_pkgrid.so libap_pkgrid_debug.so 
endif

ml: polkaGrid.mli polkaGrid.ml polkaGrid.cmi polkaGrid.cma libpolkaGrid_caml.a libpolkaGrid_caml_debug.a 
ifneq ($(HAS_OCAMLOPT),)
ml: $(call OCAMLOPT_TARGETS, polkaGrid)
endif
ifneq ($(HAS_SHARED),)
ml: dllpolkaGrid_caml.so dllpolkaGrid_caml_debug.so
endif

mlexample%.byte: mlexample.ml $(APRON_LIB)/polka%.cma $(APRON_LIB)/ppl.cma polkaGrid.cma
	$(OCAMLC) $(OCAMLFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cma gmp.cma apron.cma polka%.cma ppl.cma polkaGrid.cma $<

mlexample%.opt: mlexample.ml $(APRON_LIB)/polka%.cmxa $(APRON_LIB)/ppl.cmxa polkaGrid.cmxa
	$(OCAMLOPT) $(OCAMLOPTFLAGS) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -o $@ bigarray.cmxa gmp.cmxa apron.cmxa polka%.cmxa ppl.cmxa polkaGrid.cmxa $<

clean:
	/bin/rm -f *.[ao] *.so *.annot *.cm[xiao] *.cmx[as]
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*
	/bin/rm -fr tmp
	/bin/rm -f polkaGrid.ml polkaGrid.mli polkaGrid_caml.c Makefile.depend

distclean: clean

install:
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_LIB)
	$(INSTALL) $(CCINC_TO_INSTALL) $(APRON_INCLUDE)
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
ifeq ($(OCAMLFIND),)
	for i in $(CAML_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
endif

uninstall:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(APRON_INCLUDE)/$$i; done
	for i in $(CCLIB_TO_INSTALL); do /bin/rm -f $(APRON_LIB)/$$i; done
	for i in dllpolkaGrid_caml*.so; do /bin/rm -f $(APRON_LIB)/$$i; done
	for i in $(CCBIN_TO_INSTALL); do /bin/rm -f $(APRON_BIN)/$$i; done

dist: $(CCSRC) ap_pkgrid.texi perlscript_caml.pl polkaGrid.idl polkaGrid.ml polkaGrid.mli polkaGrid_caml.c Makefile COPYING README
	(cd ..; tar zcvf products.tgz $(^:%=products/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

#-----------------------------------
# C part
#-----------------------------------

libap_pkgrid.a: ap_pkgrid.o
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_pkgrid_debug.a: ap_pkgrid_debug.o
	$(AR) rcs $@ $^
	$(RANLIB) $@
libap_pkgrid.so: ap_pkgrid.o
	$(CXX_APRON_DYLIB) $(CXXFLAGS) -o $@ $^ -L../newpolka -lpolkaMPQ $(LDFLAGS) $(LIBS)
libap_pkgrid_debug.so: ap_pkgrid_debug.o
	$(CXX_APRON_DYLIB) $(CXXFLAGS_DEBUG) -o $@ $^ -L../newpolka -lpolkaMPQ_debug $(LDFLAGS) $(LIBS_DEBUG)

#---------------------------------------
# C rules
#---------------------------------------

OCAMLMKLIB := $(OCAMLMKLIB) -ocamlc "$(OCAMLC) -cc $(CXX)" -ocamlopt "$(OCAMLOPT) -cc $(CXX)"

dllpolkaGrid_caml.so: libpolkaGrid_caml.a
libpolkaGrid_caml.a: polkaGrid_caml.o libap_pkgrid.a
	$(OCAMLMKLIB) -o polkaGrid_caml $< -L. -lap_pkgrid -L../newpolka -lpolkaMPQ $(LDFLAGS) $(LIBS)

dllpolkaGrid_caml_debug.so: libpolkaGrid_caml_debug.a
libpolkaGrid_caml_debug.a: polkaGrid_caml_debug.o libap_pkgrid_debug.a
	$(OCAMLMKLIB) -o polkaGrid_caml_debug $< -L. -lap_pkgrid_debug -L../newpolka -lpolkaMPQ_debug $(LDFLAGS) $(LIBS_DEBUG)

#---------------------------------------
# ML rules
#---------------------------------------

polkaGrid.cma: polkaGrid.cmo libpolkaGrid_caml.a libap_pkgrid.a
	$(OCAMLMKLIB) -o polkaGrid -oc polkaGrid_caml polkaGrid.cmo -lap_pkgrid $(LIBS)

polkaGrid.cmxa: polkaGrid.a
polkaGrid.a: polkaGrid.cmx libpolkaGrid_caml.a libap_pkgrid.a
	$(OCAMLMKLIB) -o polkaGrid -oc polkaGrid_caml polkaGrid.cmx -lap_pkgrid $(LIBS)

#---------------------------------------
# IDL rules
#---------------------------------------

# generates polkaGrid.ml, polkaGrid.mli, polkaGrid_caml.c from polkaGrid.idl
%.mli %.ml %_caml.c: %.idl ../mlapronidl/macros.pl ../mlapronidl/perlscript_c.pl perlscript_caml.pl ../mlapronidl/*.idl
	mkdir -p tmp
	cp $*.idl ../mlapronidl/*.idl tmp/
	cd tmp && $(CAMLIDL) -no-include -prepro "$(PERL) ../../mlapronidl/macros.pl" $*.idl
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

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
