include ../Makefile.config
include ../vars.mk

ICFLAGS += \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(APRON_PREFIX)/include \
-Wall -Wstrict-prototypes -Wimplicit-function-declaration \
-std=c99
# -Winline -Wconversion

LCFLAGS = \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(APRON_PREFIX)/lib \
-L$(PPL_PREFIX)/lib \
-L$(CAMLIDL_PREFIX)

OCAMLINC = \
-I $(MLGMPIDL_LIB) \
-I $(APRON_PREFIX)/lib \
-I $(CAML_PREFIX)/lib/ocaml/apron

OCAMLPACKAGES = -package "apron.boxMPQ,apron.octMPQ,apron.polkaMPQ,apron.ppl,apron.polkaGrid" -linkpkg

all: C ml

# C examples

C: example1g

%g: %g.o
	$(CXX) $(CXXFLAGS) $(ICFLAGS) $(LCFLAGS) -o $@  $< -lap_ppl_debug -lppl -lgmpxx -lpolkaMPQ_debug -loctMPQ_debug -lboxMPQ_debug -lapron_debug -litvMPQ_debug -litvD_debug -lmpfr -lgmp

%g.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

# OCaml examples

ml: mlexample1.opt

mlexample%.opt: mlexample%.cmx
	$(OCAMLFIND) ocamlopt $(OCAMLINC) -o $@ $< $(OCAMLPACKAGES)

mlexample%.byte: mlexample%.cmo
	$(OCAMLFIND) ocamlc -o $@ $< $(OCAMLPACKAGES)

test.opt: test.cmx
	$(OCAMLFIND) ocamlopt -c -o $@ $< $(OCAMLPACKAGES)

%.cmo: %.ml
	$(OCAMLFIND) ocamlc -c -o $@ $< $(OCAMLPACKAGES)

%.cmx: %.ml
	$(OCAMLFIND) ocamlopt -c -o $@ $< $(OCAMLPACKAGES)

#

clean:
	rm -f example1g *.cm[ioxa] *.o mlexample1 mlexample2 mlexample3 mlexample4 mlexample5 *.opt

distclean: clean

uninstall:

dist: example1.c mlexample1.ml mlexample2.ml mlexample3.ml Makefile README
	(cd ..; tar zcvf examples.tgz $(^:%=examples/%))
