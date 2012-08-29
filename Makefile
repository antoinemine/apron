include Makefile.config

LCFLAGS = \
-Lapron -Litv -Lbox -Loctagons -Lnewpolka -Ltaylor1plus \
-L$(PPL_PREFIX)/lib -Lppl \
-Lproducts \
-L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib \
-L$(CAMLIDL_PREFIX)/lib/ocaml

all: c
ifneq ($(HAS_OCAML),)
all: ml
endif
ifneq ($(HAS_CPP),)
all: cxx
endif
ifneq ($(HAS_JAVA),)
all: java
endif

c:
	(cd num; make all)
	(cd itv; make all)
	(cd apron; make all)
	(cd newpolka; make all)
	(cd box; make all)
	(cd octagons; make MPQ D)
	(cd taylor1plus; make all)
ifneq ($(HAS_PPL),)
	(cd ppl; make)
	(cd products; make)
endif

cxx:
	(cd apronxx; make)

java:
	(cd japron; make)

ml:
	(cd mlapronidl; make all)
	(cd newpolka; make ml)
	(cd box; make ml)
	(cd octagons; make mlMPQ mlD)
	(cd taylor1plus; make ml)
ifneq ($(HAS_PPL),)
	(cd ppl; make ml)
	(cd products; make ml)
endif

.PHONY: aprontop apronppltop

aprontop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_PREFIX)/lib -I $(APRON_PREFIX)/lib -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma t1pMPQ.cma

apronppltop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_PREFIX)/lib -I $(APRON_PREFIX)/lib -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma ppl.cma polkaGrid.cma t1pMPQ.cmxa

rebuild:
ifneq ($(HAS_OCAML),)
	(cd mlapronidl; make rebuild)
	(cd newpolka; make rebuild)
	(cd box; make rebuild)
	(cd octagons; make rebuild)
	(cd taylor1plus; make rebuild)
	(cd ppl; make rebuild)
	(cd products; make rebuild)
endif

OCAMLFIND_PROTO = xxx.cma xxx.cmxa xxx.a libxxx_caml.a libxxx_caml_debug.a
ifneq ($(HAS_SHARED),)
OCAMLFIND_PROTO += dllxxx_caml.so
endif
OCAMLFIND_FILES = \
	$(patsubst %,mlapronidl/%, apron.cmi apron.cmx) \
	$(patsubst %,mlapronidl/%, $(subst xxx,apron, $(OCAMLFIND_PROTO))) \
	$(patsubst %,box/%, box.mli box.cmi box.cmx) \
	$(patsubst %,box/%, $(subst xxx,boxD, $(OCAMLFIND_PROTO))) \
	$(patsubst %,box/%, $(subst xxx,boxMPQ, $(OCAMLFIND_PROTO))) \
	$(patsubst %,box/%, $(subst xxx,boxMPFR, $(OCAMLFIND_PROTO))) \
	$(patsubst %,octagons/%, oct.mli oct.cmi oct.cmx) \
	$(patsubst %,octagons/%, $(subst xxx,octD, $(OCAMLFIND_PROTO))) \
	$(patsubst %,octagons/%, $(subst xxx,octMPQ, $(OCAMLFIND_PROTO))) \
	$(patsubst %,newpolka/%, polka.mli polka.cmi polka.cmx) \
	$(patsubst %,newpolka/%, $(subst xxx,polkaMPQ, $(OCAMLFIND_PROTO))) \
	$(patsubst %,newpolka/%, $(subst xxx,polkaRll, $(OCAMLFIND_PROTO))) \
	$(patsubst %,taylor1plus/%, t1p.mli t1p.cmi t1p.cmx) \
	$(patsubst %,taylor1plus/%, $(subst xxx,t1pD, $(OCAMLFIND_PROTO))) \
	$(patsubst %,taylor1plus/%, $(subst xxx,t1pMPQ, $(OCAMLFIND_PROTO))) \
	$(patsubst %,taylor1plus/%, $(subst xxx,t1pMPFR, $(OCAMLFIND_PROTO))) \
	$(patsubst %,mlapronidl/%.idl, scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy \
disjunction) \
	mlapronidl/apron_caml.h

ifneq ($(HAS_PPL),)
OCAMLFIND_FILES += \
	$(patsubst %,ppl/%, ppl.idl ppl.mli ppl.cmi ppl.cma ppl.cmx ppl.cmxa ppl.a libap_ppl_caml.a libap_ppl_caml_debug.a dllap_ppl_caml.so) \
	$(patsubst %,products/%, polkaGrid.idl polkaGrid.mli polkaGrid.cmi polkaGrid.cmx) \
	$(patsubst %,products/%, $(subst xxx,polkaGrid, $(OCAMLFIND_PROTO)))
endif
ifneq ($(OCAMLPACK),)
OCAMLFIND_FILES += mlapronidl/apron_ocamldoc.mli
endif
ifneq ($(HAS_TYPEREX),)
OCAMLFIND_FILES += mlapronidl/apron.cmt \
	$(patsubst %,box/%, box.cmti box.cmt) \
	$(patsubst %,octagons/%, oct.cmti oct.cmt) \
	$(patsubst %,newpolka/%, polka.cmti polka.cmt) \
	$(patsubst %,taylor1plus/%, t1p.cmti t1p.cmt)
ifneq ($(HAS_PPL),)
OCAMLFIND_FILES += \
	$(patsubst %,ppl/%, ppl.cmti ppl.cmt) \
	$(patsubst %,products/%, polkaGrid.cmti polkaGrid.cmt)
endif
endif

install:
	(cd num; make install)
	(cd itv; make install)
	(cd apron; make install)
	(cd newpolka; make install)
	(cd box; make install)
	(cd octagons; make install)
	(cd taylor1plus; make install)
ifneq ($(HAS_PPL),)
	(cd ppl; make install)
	(cd products; make install)
endif
ifneq ($(HAS_OCAML),)
ifeq ($(OCAMLFIND),)
	(cd mlapronidl; make install)
	$(INSTALLd) $(APRON_PREFIX)/bin
	if test -f aprontop; then $(INSTALL) aprontop $(APRON_PREFIX)/bin; fi
ifneq ($(HAS_PPL),)
	if test -f apronppltop; then $(INSTALL) apronppltop $(APRON_PREFIX)/bin; fi
endif
else
	$(OCAMLFIND) remove apron
	$(OCAMLFIND) install apron mlapronidl/META $(OCAMLFIND_FILES) \
mlapronidl/apron.d.cmxa mlapronidl/apron.d.a \
newpolka/polkaMPQ.d.cmxa newpolka/polkaMPQ.d.a \
newpolka/polkaRll.d.cmxa newpolka/polkaRll.d.a

endif
endif
ifneq ($(HAS_CPP),)
	(cd apronxx; make install)
endif

clean:
	(cd num; make clean)
	(cd itv; make clean)
	(cd apron; make clean)
	(cd mlapronidl; make clean)
	(cd box; make clean)
	(cd newpolka; make clean)
	(cd octagons; make clean)
	(cd taylor1plus; make clean)
	(cd ppl; make clean)
	(cd products; make clean)
	(cd apronxx; make clean)
	(cd examples; make clean)
	(cd test; make clean)
	rm -fr online tmp apron*run aprontop apronppltop

distclean: clean
	(cd num; make uninstall)
	(cd itv; make uninstall)
	(cd apron; make uninstall)
	(cd mlapronidl; make distclean)
	(cd box; make distclean)
	(cd octagons; make distclean)
	(cd taylor1plus; make distclean)
	(cd newpolka; make distclean)
	(cd ppl; make distclean)
	(cd products; make distclean)
	(cd examples; make distclean)
	(cd apronxx; make distclean)

uninstall:
	(cd num; make uninstall)
	(cd itv; make uninstall)
	(cd apron; make uninstall)
	(cd mlapronidl; make uninstall)
	(cd box; make uninstall)
	(cd newpolka; make uninstall)
	(cd octagons; make uninstall)
	(cd taylor1plus; make uninstall)
	(cd examples; make uninstall)
	(cd ppl; make uninstall)
	(cd products; make uninstall)
	(cd apronxx; make uninstall)
	(cd $(APRON_PREFIX)/bin; rm -f apron*)
ifneq ($(OCAMLFIND),)
	$(OCAMLFIND) remove apron
endif

doc:
	(cd apron; make html apron.pdf)
ifneq ($(HAS_OCAML),)
	(cd mlapronidl; make html mlapronidl.pdf)
endif
ifneq ($(HAS_CPP),)
	(cd apronxx; make doc)
endif

# make distribution, update to reflect current version

PKGNAME  = apron-0.9.11
PKGFILES = Makefile README README.windows README.mac AUTHORS COPYING Makefile.config.model Changes
PKGDIRS  = apron num itv octagons box newpolka taylor1plus ppl products mlapronidl examples test apronxx

dist:
	$(MAKE) all
	$(MAKE) doc
	mkdir -p $(PKGNAME)
	$(MAKE) $(foreach pkg,$(PKGDIRS),pkg_$(pkg))
	cp $(PKGFILES) $(PKGNAME)
	tar vczf $(PKGNAME).tgz $(PKGNAME)
	rm -rf $(PKGNAME)

# these 2 targets are for main developpers only
index.html: index.tex
	hyperlatex index.tex
online: doc index.html
	rm -fr online
	mkdir -p online
	mv index.html poster.gif flyer.pdf online
	mv apron/html online/apron
	mv mlapronidl/html online/mlapronidl
	cp apron/apron.pdf online
	cp mlapronidl/mlapronidl.pdf online
	cp examples/example1.c online
	cp examples/mlexample?.ml online
	cp Changes online

pkg_%:
	(cd $*; $(MAKE) dist)
	(cd $(PKGNAME); tar xzf ../$*.tgz)
	rm -rf $*.tgz
