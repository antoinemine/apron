include ../Makefile.config
include ../vars.mk

ICFLAGS += \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(APRON_INCLUDE) \
-I$(PPL_PREFIX)/include 

LCFLAGS = \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(APRON_LIB) \
-L$(PPL_PREFIX)/lib \
-L$(CAMLIDL_PREFIX)

OCAMLINC = \
-I $(MLGMPIDL_LIB) \
-I $(APRON_LIB)

OCAMLLDFLAGS = \
-verbose -cc "g++" \
bigarray.cma gmp.cma apron.cma boxMPQ.cma polkaMPQ.cma octD.cma ppl.cma polkaGrid.cma

OCAMLOPTLDFLAGS = \
-noautolink -verbose -ccopt "$(LCFLAGS)" \
bigarray.cmxa gmp.cmxa apron.cmxa boxMPQ.cmxa polkaMPQ.cmxa octD.cmxa ppl.cmxa polkaGrid.cmxa
all: C

# C examples
.PRECIOUS: %.c %.o %_debug.o

C: ctest1

ctest%_debug: ctest%_debug.o
	$(CXX) -g $(ICFLAGS) $(LCFLAGS) -o $@  $< \
	-lap_pkgrid_debug -lap_ppl_debug -lppl -lgmpxx -lpolkaMPQ_debug -loctMPQ_debug -lboxMPQ_debug -lapron_debug -lmpfr -lgmp

ctest%: ctest%.o
	$(CXX) $(ICFLAGS) $(LCFLAGS) -o $@  $< \
	-lap_pkgrid -lap_ppl -lppl -lgmpxx -lpolkaMPQ -loctMPQ -lboxMPQ -lapron -lmpfr -lgmp

ctest%_debug.o: ctest%.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) $(LCFLAGS) -c -o $@ $<
ctest%.o : ctest%.c
	$(CC) $(CFLAGS) $(ICFLAGS) $(LCFLAGS) -c -o $@ $<

# OCaml examples
%.opt: %.ml
	$(OCAMLFIND) ocamlopt -verbose $(OCAMLOPTFLAGS) -o $@ $< -package "apron.boxMPQ,apron.octMPQ,apron.polkaMPQ,apron.t1pMPQ,apron.ppl,apron.polkaGrid,apron.disjPk" -linkpkg

%.byte: %.ml
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) $(OCAMLLDFLAGS) -o $@ $<

clean:
	rm -f ctest1 ctest?_debug *.o *.cm[xoia] *.opt *.byte

distclean: clean

mostlyclean: clean

dist: ctest1.c Makefile README
	(cd ..; tar zcvf test.tgz $(^:%=test/%))
