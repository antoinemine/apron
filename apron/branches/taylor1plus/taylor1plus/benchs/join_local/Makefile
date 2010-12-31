include ../../../Makefile.config

ICFLAGS = \
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
-L$(CAMLIDL_PREFIX)/lib/ocaml

OCAMLINC = \
-I $(MLGMPIDL_PREFIX)/lib \
-I $(APRON_PREFIX)/lib

OCAMLLDFLAGS = \
-verbose -noautolink -ccopt "$(LCFLAGS)" \
bigarray.cma gmp.cma apron.cma \
boxD.cma octMPQ.cma polkaMPQ.cma ppl.cma \
-cclib "-lpolkaGrid_caml -lap_pkgrid -lap_ppl_caml -lap_ppl -lppl -lpolkaMPQ_caml -lpolkaMPQ -loctMPQ_caml -loctMPQ -lt1pD_caml -lt1pD -lboxD_caml -lboxD -litv -lapron_caml -lapron -lgmp_caml -lmpfr -lgmpxx -lgmp -lbigarray -lcamlidl" \
-dllib "-lpolkaGrid_caml -lap_ppl_caml -lpolkaMPQ_caml -loctMPQ_caml -lboxD_caml -lt1pD_caml -lapron_caml -lgmp_caml -lbigarray"

OCAMLOPTLDFLAGS = \
-noautolink -verbose -ccopt "$(LCFLAGS)" \
bigarray.cmxa gmp.cmxa apron.cmxa \
boxD.cmxa polkaMPQ.cmxa ppl.cmxa \
-cclib "-lpolkaGrid_caml -lap_pkgrid -lap_ppl_caml -lap_ppl -lppl -lpolkaMPQ_caml -lpolkaMPQ -loctMPQ_caml -loctMPQ -lt1pD_caml -lt1pD -lboxD_caml -lboxD -litv -lapron_caml -lapron -lgmp_caml -lmpfr -lgmpxx -lgmp -lbigarray -lcamlidl"

all: C ml

# C examples

C: example1g

%g: %g.o
	$(CXX) $(CXXFLAGS) $(ICFLAGS) $(LCFLAGS) -o $@  $< -lapron_ppl -lppl -lgmpxx -lpolkag -loctQg -lboxD -lapron -litvMPQ -litvdbl -lmpfr -lgmp

%g.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

# OCaml examples

ml: mltest.opt join_ub_*.ml

join_ub_%.opt: join_ub_%.cmx
	$(OCAMLOPT) -cc "g++ -pg" $(OCAMLOPTFLAGS) $(OCAMLINC) $(OCAMLOPTLDFLAGS) -o $@ $<

join_ub_%.byte: join_ub_%.cmo
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) $(OCAMLLDFLAGS) -g -o $@ $<

mltest.opt: mltest.cmx
	$(OCAMLOPT) -cc "g++" $(OCAMLOPTFLAGS) $(OCAMLINC) $(OCAMLOPTLDFLAGS) -o $@ $<

mltest.byte: mltest.cmo
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) $(OCAMLLDFLAGS) -g -o $@ $<

test.opt: test.cmx
	$(OCAMLOPT) -cc "g++" $(OCAMLOPTFLAGS) $(OCAMLINC) $(OCAMLOPTLDFLAGS) -o $@ $<

%.cmo: %.ml
	$(OCAMLC) $(OCAMLFLAGS) $(OCAMLINC) -c -o $@ $<

%.cmx: %.ml
	$(OCAMLOPT) $(OCAMLOPTFLAGS) $(OCAMLINC)  -c -o $@ $<

#

clean:
	rm -f example1g *.cm[ioxa] *.o mlexample1 mlexample2 mlexample3 mlexample4 mlexample5 *.opt

distclean: clean

dist: example1.c mlexample1.ml mlexample2.ml mlexample3.ml Makefile README
	(cd ..; tar zcvf examples.tgz $(^:%=examples/%))
