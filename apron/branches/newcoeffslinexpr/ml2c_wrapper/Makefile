
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
-I../box \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(CAML_LIB) -I$(CAMLIDL_LIB)

LDFLAGS = -L$(APRON_PREFIX)/lib/apron -L.. -L../mlapronidl \
	-L$(CAML_LIB) -L$(CAMLIDL_LIB) -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -L$(SITE-LIB-gmp) \
	-lapron_caml.callback -lapron -lgmp_caml.callback -lcamlidl -lunix -lbigarray -lcamlstr -lasmrun -lgmp -lmpfr

LDFLAGS_DEBUG = -L$(APRON_PREFIX)/lib/apron -L.. -L../mlapronidl \
	-L$(CAML_LIB) -L$(CAMLIDL_LIB) -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -L$(SITE-LIB-gmp) \
	-lapron_caml.callback.d -lapron.d -lgmp_caml.callback -lcamlidl -lunix -lbigarray -lcamlstr -lasmrun -lgmp -lmpfr

#---------------------------------------
# Files
#---------------------------------------

ML_MODULES = caml_abstract_domain

#---------------------------------------
# Rules
#---------------------------------------

all: generator libml2c_wrapper test test_domain
debug: 
prof: 

essai:
	echo "$(ICFLAGS)"

generator: generator.ml
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -o $@ $^

caml_abstract_domain_obj.o: ../mlapronidl/apron.callback.cmxa $(ML_MODULES:%=%.cmx)
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) \
	-predicates callback -package $(REQ_PKG) -linkpkg -output-obj -o $@ $^

caml_abstract_domain_obj.d.o: ../mlapronidl/apron.callback.d.cmxa $(ML_MODULES:%=%.d.cmx)
	$(OCAMLFIND) ocamlopt -g $(OCAMLOPTFLAGS) \
	-predicates callback,debug -package $(REQ_PKG) -linkpkg -output-obj -o $@ $^

ml2c_wrapper.h: ml2c_wrapper_preamble.h generator
	./generator -preamble-h $< -output-h $@

ml2c_wrapper.c: ml2c_wrapper_preamble.c ml2c_wrapper_postamble.c ml2c_wrapper.h generator
	./generator -preamble-c ml2c_wrapper_preamble.c \
	-postamble-c ml2c_wrapper_postamble.c \
	-output-c $@

libml2c_wrapper: caml_abstract_domain.cmx caml_abstract_domain_obj.o ml2c_wrapper.o
	ocamlmklib -verbose $(LDFLAGS) -o ml2c_wrapper $^ 

libml2c_wrapper.d: caml_abstract_domain.d.cmx caml_abstract_domain_obj.d.o ml2c_wrapper.d.o
	ocamlmklib -ccopt "-g" -cclib "-g" -verbose $(LDFLAGS_DEBUG) -o ml2c_wrapper.d $^ 

test: libml2c_wrapper.d test.d.o
	$(CC) $(CFLAGS_DEBUG) $(LDFLAGS_DEBUG) -L. -lml2c_wrapper.d -o $@ test.d.o

test_domain: libml2c_wrapper.d test_domain.d.o
	$(CC) $(CFLAGS_DEBUG) $(LDFLAGS_DEBUG) -L. -lml2c_wrapper.d -o $@ test_domain.d.o

#test: caml_abstract_domain_obj.d.o ml2c_wrapper.d.o test.d.o
#	$(CC) $(LDFLAGS) -o $@ $^ 

clean:
	/bin/rm -f *.o *.a *.so *.cmi *.cmo *.cmx *.cmxa *.cma *.annot *~
	/bin/rm -f generator test test_domain ml2c_wrapper.h ml2c_wrapper.c

%.cmi: %.mli
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -predicates callback -package $(REQ_PKG) -c $<

%.cmo: %.ml %.cmi
	$(OCAMLFIND) ocamlc $(OCAMLFLAGS) -predicates callback -package $(REQ_PKG) -c $<

%.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt $(OCAMLOPTFLAGS) -predicates callback -package $(REQ_PKG) -c $<

%.d.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt -g $(OCAMLOPTFLAGS) -predicates callback,debug -package $(REQ_PKG) -c -o $@ $<

%.p.cmx: %.ml %.cmi
	$(OCAMLFIND) ocamlopt -p $(OCAMLOPTFLAGS) -predicates callback,gprof -package $(REQ_PKG) -c -o $@ $<

%.o: %.c 
	$(CC) $(CFLAGS) $(ICFLAGS) -DMLGMPIDL_CALLBACK -DMLAPRONIDL_CALLBACK -c -o $@ $<
%.d.o: %.c 
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DMLGMPIDL_CALLBACK -DMLAPRONIDL_CALLBACK -c -o $@ $<
%.p.o: %.c 
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -DMLGMPIDL_CALLBACK -DMLAPRONIDL_CALLBACK -c -o $@ $<

#---------------------------------------
# Dependencies
#---------------------------------------

depend: $(ML_MODULES:%=%.ml) $(ML_MODULES:%=%.mli)
	$(OCAMLFIND) ocamldep $^ >Makefile.depend

-include Makefile.depend


