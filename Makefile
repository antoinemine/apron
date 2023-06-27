include Makefile.config
SRCROOT = .
include vars.mk
include version.mk
PKGNAME = apron

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
	(cd num; $(MAKE) all)
	(cd itv; $(MAKE) all)
	(cd apron; $(MAKE) all)
	(cd newpolka; $(MAKE) all)
	(cd box; $(MAKE) all)
	(cd octagons; $(MAKE) MPQ D)
	(cd taylor1plus; $(MAKE) all)
ifneq ($(HAS_PPL),)
	(cd ppl; $(MAKE))
	(cd products; $(MAKE))
endif
ifneq ($(HAS_PPLITE),)
	(cd pplite; $(MAKE))
endif
	(cd avoct; $(MAKE) MPQ D)
ifneq ($(HAS_GLPK),)
	(cd fppol; $(MAKE) all) 
endif

cxx:
	(cd apronxx; $(MAKE))

java:
	(cd japron; $(MAKE))

ml:
	(cd mlapronidl; $(MAKE) INCLUDE_DEPEND=no depend; $(MAKE) all)
	(cd newpolka; $(MAKE) ml)
	(cd box; $(MAKE) ml)
	(cd octagons; $(MAKE) mlMPQ mlD)
	(cd taylor1plus; $(MAKE) ml)
ifneq ($(HAS_PPL),)
	(cd ppl; $(MAKE) ml)
	(cd products; $(MAKE) ml)
endif
ifneq ($(HAS_PPLITE),)
	(cd pplite; $(MAKE) ml)
endif
	(cd avoct; $(MAKE) mlMPQ mlD)
ifneq ($(HAS_GLPK),)
	(cd fppol; $(MAKE) allml) 
endif

aprontop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma t1pMPQ.cma avoMPQ.cma

apronppltop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma ppl.cma polkaGrid.cma t1pMPQ.cmxa avoMPQ.cma

apronpplitetop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma pplite.cma t1pMPQ.cmxa avoMPQ.cma

apronglpktop:
	$(OCAMLMKTOP) -I $(MLGMPIDL_LIB) -I $(APRON_LIB) -verbose -o $@ \
	bigarray.cma gmp.cma apron.cma boxMPQ.cma octMPQ.cma polkaMPQ.cma t1pMPQ.cma avoMPQ.cma fppMPQ.cma

rebuild:
	@echo "$(MAKE) rebuild is no longer necessary"

OCAMLFIND_PROTO = xxx.cma $(call OCAMLOPT_TARGETS, xxx) libxxx_caml.a
ifneq ($(HAS_DEBUG),)
OCAMLFIND_PROTO += $(call OCAMLOPT_TARGETS, xxx.d)  libxxx_caml_debug.a
endif
ifneq ($(HAS_SHARED),)
OCAMLFIND_PROTO += dllxxx_caml.$(EXT_DLL)
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
	$(patsubst %,avoct/%, avo.mli avo.cmi avo.cmx) \
	$(patsubst %,avoct/%, $(subst xxx,avoD, $(OCAMLFIND_PROTO))) \
	$(patsubst %,avoct/%, $(subst xxx,avoMPQ, $(OCAMLFIND_PROTO))) \
	$(patsubst %,mlapronidl/%.idl, scalar interval coeff \
dim linexpr0 lincons0 generator0 texpr0 tcons0 manager abstract0 \
var environment linexpr1 lincons1 generator1 texpr1 tcons1 abstract1 policy \
disjunction) \
	mlapronidl/apron_caml.h

ifneq ($(HAS_PPL),)
OCAMLFIND_FILES += \
	$(patsubst %,ppl/%, ppl.idl ppl.mli ppl.cmi ppl.cma ppl.cmx $(call OCAMLOPT_TARGETS, ppl) libap_ppl_caml.a dllap_ppl_caml.$(EXT_DLL)) \
	$(patsubst %,products/%, polkaGrid.idl polkaGrid.mli polkaGrid.cmi polkaGrid.cmx) \
	$(patsubst %,products/%, $(subst xxx,polkaGrid, $(OCAMLFIND_PROTO)))
endif
ifneq ($(HAS_PPLITE),)
OCAMLFIND_FILES += \
	$(patsubst %,pplite/%, pplite.idl pplite.mli pplite.cmi pplite.cma pplite.cmx $(call OCAMLOPT_TARGETS, pplite) libap_pplite_caml.a dllap_pplite_caml.$(EXT_DLL))
endif
ifneq ($(HAS_GLPK),)
OCAMLFIND_FILES += \
	$(patsubst %,fppol/%, fpp.mli fpp.cmi fpp.cmx) \
	$(patsubst %,fppol/%, $(subst xxx,fppD, $(OCAMLFIND_PROTO)))  
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
ifneq ($(HAS_PPLITE),)
OCAMLFIND_FILES += \
	$(patsubst %,pplite/%, pplite.cmti pplite.cmt)
endif
	$(patsubst %,avoct/%, avo.cmti avo.cmt)
ifneq ($(HAS_GLPK),)
OCAMLFIND_FILES += \
	$(patsubst %,fppol/%, fpp.cmti fpp.cmt) 
endif
endif

install: all
	(cd num; $(MAKE) install)
	(cd itv; $(MAKE) install)
	(cd apron; $(MAKE) install)
	(cd newpolka; $(MAKE) install)
	(cd box; $(MAKE) install)
	(cd octagons; $(MAKE) install)
	(cd taylor1plus; $(MAKE) install)
ifneq ($(HAS_PPL),)
	(cd ppl; $(MAKE) install)
	(cd products; $(MAKE) install)
endif
ifneq ($(HAS_PPLITE),)
	(cd pplite; $(MAKE) install)
endif
	(cd avoct; $(MAKE) install)
ifneq ($(HAS_GLPK),)
	(cd fppol; $(MAKE) install) 
endif
ifneq ($(HAS_OCAML),)
ifeq ($(OCAMLFIND),)
	(cd mlapronidl; $(MAKE) install)
	$(INSTALLd) $(APRON_BIN)
	if test -f aprontop; then $(INSTALL) aprontop $(APRON_BIN); fi
ifneq ($(HAS_PPL),)
	if test -f apronppltop; then $(INSTALL) apronppltop $(APRON_BIN); fi
endif
ifneq ($(HAS_PPLITE),)
	if test -f apronpplitetop; then $(INSTALL) apronpplitetop $(APRON_BIN); fi
endif
ifneq ($(HAS_GLPK),)
		if test -f apronglpktop; then $(INSTALL) apronglpktop $(APRON_BIN); fi
endif
else
	$(OCAMLFIND) remove apron
	$(OCAMLFIND) install apron mlapronidl/META $(OCAMLFIND_FILES)
endif
endif
ifneq ($(HAS_CPP),)
	(cd apronxx; $(MAKE) install)
endif
ifneq ($(HAS_JAVA),)
	(cd japron; $(MAKE) install)
endif

ifneq ($(OCAMLFIND),)
ml: mlapronidl/META
mlapronidl/META: mlapronidl/META.in mlapronidl/META.ppl.in mlapronidl/META.pplite.in
	$(SED) -e "s!@VERSION@!$(VERSION_STR)!g;" $< > $@;
  ifneq ($(HAS_PPL),)
	cat mlapronidl/META.ppl.in >> $@;
  endif
  ifneq ($(HAS_PPLITE),)
	cat mlapronidl/META.pplite.in >> $@;
  endif
	$(SED) -e '/^[[:space:]]*archive(byte)/ { p; s ) ,plugin) ;}' -i.bak $@;
  ifneq ($(HAS_NATIVE_PLUGINS),)
	$(SED) -e '/^[[:space:]]*archive(native)/ { p; s ) ,plugin) ;'\
	'            s \.cmxa\" .cmxs\" ;}' -i.bak $@;
  endif
	test -f $@.bak && rm -f $@.bak
endif

clean:
	(cd num; $(MAKE) clean)
	(cd itv; $(MAKE) clean)
	(cd apron; $(MAKE) clean)
	(cd mlapronidl; $(MAKE) clean)
	(cd box; $(MAKE) clean)
	(cd newpolka; $(MAKE) clean)
	(cd octagons; $(MAKE) clean)
	(cd taylor1plus; $(MAKE) clean)
	(cd ppl; $(MAKE) clean)
	(cd pplite; $(MAKE) clean)
	(cd products; $(MAKE) clean)
	(cd avoct; $(MAKE) clean)
	(cd fppol; $(MAKE) clean) 
	(cd apronxx; $(MAKE) clean)
	(cd examples; $(MAKE) clean)
	(cd test; $(MAKE) clean)
	(cd japron; $(MAKE) clean)
	rm -fr online tmp apron*run aprontop apronppltop apronpplitetop apronglpktop
	rm -f mlapronidl/META

distclean: clean
	rm -f Makefile.config

uninstall:
	(cd num; $(MAKE) uninstall)
	(cd itv; $(MAKE) uninstall)
	(cd apron; $(MAKE) uninstall)
	(cd mlapronidl; $(MAKE) uninstall)
	(cd box; $(MAKE) uninstall)
	(cd newpolka; $(MAKE) uninstall)
	(cd octagons; $(MAKE) uninstall)
	(cd taylor1plus; $(MAKE) uninstall)
	(cd examples; $(MAKE) uninstall)
	(cd ppl; $(MAKE) uninstall)
	(cd pplite; $(MAKE) uninstall)
	(cd products; $(MAKE) uninstall)
	(cd avoct; $(MAKE) uninstall)
	(cd fppol; $(MAKE) uninstall) 
	(cd apronxx; $(MAKE) uninstall)
	(cd japron; $(MAKE) uninstall)
	(cd $(APRON_BIN); rm -f apron*)
ifneq ($(OCAMLFIND),)
	$(OCAMLFIND) remove apron
endif

doc:
	(cd apron; $(MAKE) html apron.pdf)
ifneq ($(HAS_OCAML),)
	(cd mlapronidl; $(MAKE) html)
endif
ifneq ($(HAS_CPP),)
	(cd apronxx; $(MAKE) doc)
endif
ifneq ($(HAS_JAVA),)
	(cd japron; $(MAKE) doc)
endif

# make distribution, update to reflect current version

PKG  = $(PKGNAME)-$(VERSION_STR)
PKGFILES = Makefile README README.windows README.mac AUTHORS COPYING Makefile.config.model Changes configure vars.mk ocamlpack
PKGDIRS  = apron num itv octagons box newpolka taylor1plus ppl pplite products avoct fppol mlapronidl examples test apronxx japron

dist:
	$(MAKE) all
	mkdir -p $(PKG)
	$(MAKE) $(foreach pkg,$(PKGDIRS),pkg_$(pkg))
	cp $(PKGFILES) $(PKG)
	tar vczf $(PKG).tgz $(PKG)
	rm -rf $(PKG)

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
	(cd $(PKG); tar xzf ../$*.tgz)
	rm -rf $*.tgz

.NOTPARALLEL:

Makefile.config:
	@echo
	@echo "***"
	@echo "*** Run ./configure first to generate a Makefile.config"
	@echo "***"
	@echo
	@exit 1
