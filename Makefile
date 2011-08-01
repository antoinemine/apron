include Makefile.config

CCLIB_TO_INSTALL = libapron.a libapron.d.a libapron.p.a 
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += libapron.so libapron.d.so libapron.p.so
endif

LCFLAGS = \
-L$(PPL_PREFIX)/lib \
-L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib \
-L$(CAMLIDL_PREFIX)/lib/ocaml

all: c_all
debug: c_debug
prof: c_prof
ifneq ($(HAS_OCAML),)
all: ml_all
debug: ml_debug
prof: ml_prof
endif
ifneq ($(HAS_CPP),)
all: cxx_all
debug: cxx_debug
prof: cxx_prof
endif
ifneq ($(HAS_JAVA),)
all: java_all
endif

SUBDIR_C = num apron box polka
SUBDIR_ALL = $(SUBDIR_C) 
ifneq ($(HAS_PPL),)
SUBDIR_ALL += ppl products
endif
ifneq ($(HAS_OCAML),)
SUBDIR_ALL += mlapronidl
endif
ifneq ($(HAS_CPP),)
SUBDIR_ALL += apronxx
endif
ifneq ($(HAS_JAVA),)
SUBDIR_ALL += java
endif

c_all:
	for i in $(SUBDIR_C); do make -C $$i all; done
cxx_all:
	make -C apronxx all
ml_all:
	make -C mlapronidl all

c_debug:
	for i in $(SUBDIR_C); do make -C $$i debug; done
cxx_debug:
	make -C apronxx debug
ml_debug:
	make -C mlapronidl debug
c_prof:
	for i in $(SUBDIR_C); do make -C $$i prof; done
cxx_prof:
	make -C apronxx prof
ml_prof:
	make -C mlapronidl prof
depend:
	for i in $(SUBDIR_ALL); do make -C $$i depend; done
src:
	for i in $(SUBDIR_ALL); do make -C $$i src; done

install: $(CCLIB_TO_INSTALL)
	$(INSTALL) -d $(APRON_PREFIX)/lib/apron
	$(INSTALL) $(CCLIB_TO_INSTALL) $(APRON_PREFIX)/lib/apron
	for i in $(SUBDIR_C); do make -C $$i install; done
ifneq ($(HAS_PPL),)
	make -C ppl install
	make -C products install
endif
ifneq ($(HAS_OCAML),)
	make -C mlapronidl install
endif
ifneq ($(HAS_CPP),)
	make -C apronxx install
endif
uninstall:
	rm -fr $(APRON_PREFIX)/lib/apron
	rm -fr $(APRON_PREFIX)/include/apron

clean:
	for i in $(SUBDIR_ALL); do make -C $$i clean; done
mostlyclean:
	for i in $(SUBDIR_ALL); do make -C $$i mostlyclean; done
distclean:
	for i in $(SUBDIR_ALL); do make -C $$i distclean; done

doc:
	make -C apron html apron.pdf
ifneq ($(HAS_OCAML),)
	make -C mlapronidl html mlapronidl.pdf
endif
ifneq ($(HAS_CPP),)
	make -C apronxx doc
endif

# ######################################################################
# C part
# ######################################################################

define generate-sublib
.PHONY: $(1)/lib$(1)$(2).a
$(1)/lib$(1)$(2).a:
	make -C $(1) lib$(1)$(2).a
endef
$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),)))
$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),.d)))
$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),.p)))

define generate-clib
libapron$(1).a: $(foreach M,$(SUBDIR_C),$(M)/lib$(M)$(1).a)
	mkdir -p tmp
	rm -f tmp/*
	(cd tmp; for i in $$^; do $(AR) x ../$$$$i ; done)
	$(AR) rcs $$@ tmp/*.o
	$(RANLIB) $$@
ifneq ($(HAS_SHARED),)
libapron$(1).so: $(foreach M,$(SUBDIR_C),$(M)/lib$(M)$(1).a)
	mkdir -p tmp
	rm -f tmp/*
	(cd tmp; for i in $$^; do $(AR) x ../$$$$i ; done)
	$(CC) $(CFLAGS) -shared -o $$@ -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp tmp/*.o 
endif
endef
$(eval $(call generate-clib,))
$(eval $(call generate-clib,.d))
$(eval $(call generate-clib,.p))



# make distribution, update to reflect current version

PKGNAME  = apron-1.0
PKGFILES = Makefile README README.windows README.mac AUTHORS COPYING Makefile.config.model Changes
PKGDIRS  = apron num itv octagons box polka mlapronidl examples test apronxx #ppl products 

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
