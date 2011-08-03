
include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)

SITE-LIB = $(shell $(OCAMLFIND) printconf destdir)
PKG-NAME = apron
SITE-LIB-PKG = $(SITE-LIB)/$(PKG-NAME)
REQ_PKG = bigarray,str,gmp

OCAMLFLAGS += -I ../mlapronidl 
OCAMLOPTFLAGS += -I ../mlapronidl

SITE-LIB-gmp = $(shell $(OCAMLFIND) query gmp)

#---------------------------------------
# CAML part
#---------------------------------------

OCAMLCCOPT = -ccopt -L$(SITE-LIB-PKG)

#---------------------------------------
# C part
#---------------------------------------
ICFLAGS = \
-I$(SITE-LIB-gmp) \
-I../apron \
-I../mlapronidl \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_LIB) -I$(CAMLIDL_LIB)

LDFLAGS = -L.. -L../mlapronidl -L$(CAML_LIB) -L$(CAMLIDL_LIB) -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -L$(SITE-LIB-gmp) \
	-lasmrun -lcamlidl -lgmp -lmpfr -lgmp_caml \
	-lapron -lapron_caml -lunix -lbigarray -lcamlstr

#---------------------------------------
# Files
#---------------------------------------

ML_MODULES = caml_abstract_domain

#---------------------------------------
# Rules
#---------------------------------------

all: generator test
debug: 
prof: 

essai:
	echo "$(ICFLAGS)"

generator: generator.ml
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -o $@ $^

caml_abstract_domain_obj.o: ../mlapronidl/apron.cmxa $(ML_MODULES:%=%.cmx)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) \
	-package $(REQ_PKG) -linkpkg -output-obj -o $@ $^

caml_abstract_domain_wrapper.c: caml_abstract_domain_wrapper_preamble.c generator
	cp caml_abstract_domain_wrapper_preamble.c $@
	./generator >> $@

test: caml_abstract_domain_obj.o caml_abstract_domain_wrapper.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	/bin/rm -f *.o *.a *.so *.cmi *.cmo *.cmx *.cmxa *.cma *.annot 
	/bin/rm -f generator test caml_abstract_domain_wrapper.c

%.cmi: %.mli
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -c $<

%.cmo: %.ml %.cmi
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -package $(REQ_PKG) -c $<

%.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -package $(REQ_PKG) -c $<

%.p.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -package $(REQ_PKG) -c -o $@ $<

%.o: %.c 
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
%.d.o: %.c 
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
%.p.o: %.c 
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

#---------------------------------------
# Dependencies
#---------------------------------------

depend: $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli)
	$(OCAMLFIND) ocamldep $^ >Makefile.depend

-include Makefile.depend


