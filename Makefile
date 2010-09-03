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
	(cd mlapronidl; make install)
	$(INSTALLd) $(APRON_PREFIX)/bin
	if test -f aprontop; then $(INSTALL) aprontop $(APRON_PREFIX)/bin; fi
ifneq ($(HAS_PPL),)
	if test -f aprontop; then $(INSTALL) apronppltop $(APRON_PREFIX)/bin; fi
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
	(cd ppl; make clean)
	(cd products; make clean)
	(cd apronxx; make clean)
	(cd examples; make clean)
	(cd test; make clean)
	rm -fr online tmp apron*run aprontop apronppltop

mostlyclean: clean
	(cd mlapronidl; make mostlyclean)
	(cd box; make mostlyclean)
	(cd octagons; make mostlyclean)
	(cd newpolka; make mostlyclean)
	(cd ppl; make mostlyclean)
	(cd products; make mostlyclean)
	(cd apronxx; make mostlyclean)

uninstall: distclean

distclean:
	(cd num; make distclean)
	(cd itv; make distclean)
	(cd apron; make distclean)
	(cd mlapronidl; make distclean)
	(cd box; make distclean)
	(cd newpolka; make distclean)
	(cd octagons; make distclean)
	(cd taylor1plus; make uninstall)
	(cd examples; make distclean)
	(cd ppl; make distclean)
	(cd products; make distclean)
	(cd apronxx; make distclean)
	(cd $(APRON_PREFIX)/bin; rm -f apron*)

doc:
	(cd apron; make html apron.pdf)
ifneq ($(HAS_OCAML),)
	(cd mlapronidl; make html mlapronidl.pdf)
endif
ifneq ($(HAS_CPP),)
	(cd apronxx; make doc)
endif

# make distribution, update to reflect current version

PKGNAME  = apron-0.9.10
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
