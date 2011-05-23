include Makefile.config

LCFLAGS = \
-L$(PPL_PREFIX)/lib \
-L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib \
-L$(CAMLIDL_PREFIX)/lib/ocaml

all: c
ifneq ($(HAS_OCAML),)
all: ml
endif
ifneq ($(HAS_CPP),)
all: cxx
endif

SUBDIR_C = num apron box polka
SUBDIR_ALL = $(SUBDIR_C) 
ifneq ($(HAS_OCAML),)
SUBDIR_ALL += mlapronidl
endif
ifneq ($(HAS_CPP),)
SUBDIR_ALL += apronxx
endif
ifneq ($(HAS_JAVA),)
SUBDIR_ALL += java
endif

depend:
	for i in $(SUBDIR_ALL); do make -C $$i depend; done

src:
	make -C num src
	make -C apron src
	make -C box src
	make -C polka src
	make -C mlapronidl src

c:
	make -C num 
	make -C apron
	make -C box 
	make -C polka


cxx:
	make -C apronxx

ml:
	make -C mlapronidl

.PHONY: aprontop apronppltop

rebuild:
ifneq ($(HAS_OCAML),)
	make -C mlapronidl rebuild
	make -C polka rebuild
	make -C box rebuild
	make -C octagons rebuild
	make -C ppl rebuild
	make -C products rebuild
endif

install:
	make -C num install
	make -C apron install
	make -C polka install
	make -C box install
	make -C octagons install
ifneq ($(HAS_PPL),)
	make -C ppl install
	make -C products install
endif
ifneq ($(HAS_OCAML),)
	make -C mlapronidl install
	$(INSTALLd) $(APRON_PREFIX)/bin
	if test -f aprontop; then $(INSTALL) aprontop $(APRON_PREFIX)/bin; fi
ifneq ($(HAS_PPL),)
	if test -f aprontop; then $(INSTALL) apronppltop $(APRON_PREFIX)/bin; fi
endif
endif
ifneq ($(HAS_CPP),)
	make -C apronxx install
endif

uninstall:

clean:
	make -C num clean
	make -C apron clean
	make -C box clean
	make -C polka clean
	make -C octagons clean
	make -C ppl clean
	make -C products clean
	make -C apronxx clean
	make -C mlapronidl clean
	make -C examples clean
	make -C test clean
	rm -fr online tmp apron*run aprontop apronppltop

mostlyclean:
	make -C num mostlyclean
	make -C apron mostlyclean
	make -C box mostlyclean
	make -C polka mostlyclean
	make -C octagons mostlyclean
	make -C ppl mostlyclean
	make -C products mostlyclean
	make -C apronxx mostlyclean
	make -C mlapronidl mostlyclean
	make -C examples mostlyclean
	make -C test mostlyclean

distclean:
	make -C num distclean
	make -C apron distclean
	make -C box distclean
	make -C polka distclean
	make -C octagons distclean
	make -C ppl distclean
	make -C products distclean
	make -C apronxx distclean
	make -C mlapronidl distclean
	make -C examples distclean
	make -C test mostlyclean

doc:
	make -C apron html apron.pdf
ifneq ($(HAS_OCAML),)
	make -C mlapronidl html mlapronidl.pdf
endif
ifneq ($(HAS_CPP),)
	make -C apronxx doc
endif

# C part
MOD = num apron box polka

define generate-sublib
.PHONY: $(1)/lib$(1)$(2).a
$(1)/lib$(1)$(2).a:
	make -C $(1) lib$(1)$(2).a
endef
$(foreach M,$(MOD),$(eval $(call generate-sublib,$(M),)))
$(foreach M,$(MOD),$(eval $(call generate-sublib,$(M),_debug)))
$(foreach M,$(MOD),$(eval $(call generate-sublib,$(M),_prof)))

define generate-clib
libapron$(1).a: $(foreach M,$(MOD),$(M)/lib$(M)$(1).a)
	mkdir -p tmp
	rm -f tmp/*
	(cd tmp; for i in $$^; do $(AR) x ../$$$$i ; done)
	$(AR) rcs $$@ tmp/*.o
	$(RANLIB) $$@
ifneq ($(HAS_SHARED),)
libapron$(1).so: $(foreach M,$(MOD),$(M)/lib$(M)$(1).a)
	mkdir -p tmp
	rm -f tmp/*
	(cd tmp; for i in $$^; do $(AR) x ../$$$$i ; done)
	$(CC) $(CFLAGS) -shared -o $$@ tmp/*.o -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp
endif
endef
$(eval $(call generate-clib,))
$(eval $(call generate-clib,_debug))
$(eval $(call generate-clib,_prof))



# make distribution, update to reflect current version

PKGNAME  = apron-0.9.10
PKGFILES = Makefile README README.windows README.mac AUTHORS COPYING Makefile.config.model Changes
PKGDIRS  = apron num itv octagons box polka ppl products mlapronidl examples test apronxx

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
