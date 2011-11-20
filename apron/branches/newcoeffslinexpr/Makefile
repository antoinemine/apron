include Makefile.config

CCLIB_TO_INSTALL_s = libapron.a
ifneq ($(APRON_DEBUG),)
CCLIB_TO_INSTALL_s += libapron.d.a
endif
ifneq ($(APRON_PROF),)
CCLIB_TO_INSTALL_s += libapron.p.a
endif
CCLIB_TO_INSTALL = $(CCLIB_TO_INSTALL_s)
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += $(CCLIB_TO_INSTALL_s:%.a=%.so)
endif

LCFLAGS = \
-L$(PPL_PREFIX)/lib \
-L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib \
-L$(CAMLIDL_LIB)

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

SUBDIR_C_DOM = box octagon polka
ifneq ($(HAS_PPL),)
SUBDIR_C_DOM += ppl #products
endif
SUBDIR_C = apron $(SUBDIR_C_DOM)

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
ifneq ($(HAS_PPL),)
SUBDIR_ALL += ppl
endif

c_all:
	for i in $(SUBDIR_C); do make -C $$i all; done
	$(MAKE) libapron.a
ifneq ($(HAS_SHARED),)
	$(MAKE) libapron.so
endif
c_debug:
	for i in $(SUBDIR_C); do make -C $$i debug; done
	$(MAKE) libapron.d.a
ifneq ($(HAS_SHARED),)
	$(MAKE) libapron.d.so
endif
c_prof:
	for i in $(SUBDIR_C); do make -C $$i prof; done
	$(MAKE) libapron.p.a
ifneq ($(HAS_SHARED),)
	$(MAKE) libapron.p.so
endif

#c_all: libapron.a
#c_debug: libapron.d.a
#c_prof: libapron.p.a
#ifneq ($(HAS_SHARED),)
#c_all: libapron.so
#c_debug: libapron.d.so
#c_prof: libapron.p.so
#endif

cxx_all: libapronxx.a
cxx_debug: libapronxx.d.a
cxx_prof: libapronxx.p.a
ifneq ($(HAS_SHARED),)
cxx_all: libapronxx.so
cxx_debug: libapronxx.d.so
cxx_prof: libapronxx.p.so
endif

ml_all:
	make -C mlapronidl all
ml_debug:
	make -C mlapronidl debug
ml_prof:
	make -C mlapronidl prof

depend:
	for i in $(SUBDIR_ALL); do make -C $$i depend; done
src:
	for i in $(SUBDIR_ALL); do make -C $$i src; done

install: $(CCLIB_TO_INSTALL)
	$(INSTALL) -d $(APRON_PREFIX)/lib/apron
	for i in $(CCLIB_TO_INSTALL); do \
		$(INSTALL) $$i $(APRON_PREFIX)/lib/apron; \
	done
	for i in $(SUBDIR_C); do make -C $$i install; done
ifneq ($(HAS_PPL),)
	make -C ppl install
#	make -C products install
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
	make -C ctest clean
	rm -f *.a *.so
mostlyclean: 
	for i in $(SUBDIR_ALL); do make -C $$i mostlyclean; done
	make -C ctest mostlyclean
	rm -f *.a *.so
distclean:
	for i in $(SUBDIR_ALL); do make -C $$i distclean; done
	make -C ctest distclean
	rm -f *.a *.so
	rm -fr tmp
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

#define generate-sublib
#$(1)/lib$(1)$(2).a:
#	make -C $(1) src lib$(1)$(2).a
#endef
#$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),)))
#$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),.d)))
#$(foreach M,$(SUBDIR_C),$(eval $(call generate-sublib,$(M),.p)))

define generate-clib
libapron$(1).a: $(foreach M,$(SUBDIR_C),$(M)/lib$(M)$(1).a)
	$(AR) rcs $$@ $$(foreach d,$(SUBDIR_C),$$(patsubst %,$$(d)/%,$$(filter %.o,$$(shell $(AR) -t $$(d)/lib$$(d)$(1).a))))
	$(RANLIB) $$@
libapron$(1).so: $(foreach M,$(SUBDIR_C),$(M)/lib$(M)$(1).a)
	$(CC) $(CFLAGS) -shared -o $$@ -L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp $$(foreach d,$(SUBDIR_C),$$(patsubst %,$$(d)/%,$$(filter %.o,$$(shell $(AR) -t $$(d)/lib$$(d)$(1).a))))
endef
$(eval $(call generate-clib,,))
$(eval $(call generate-clib,.d))
$(eval $(call generate-clib,.p))

# make distribution, update to reflect current version

PKGNAME  = apron-1.0
PKGFILES = Makefile README README.windows README.mac AUTHORS COPYING Makefile.config.model Changes
PKGDIRS  = apron box polka mlapronidl octagons examples test apronxx #ppl products 

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
