include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib

#---------------------------------------
# Files
#---------------------------------------

TMPL = $(APRON_NUMTYPES)

H_TMPL_FILES_XXX = boundXXX.h itvXXX.h eitvXXX.h 
H_TMPL_FILES_tmpl = num_types.h.tmpl numMPQ.h.tmpl num_conv.h.tmpl
H_TMPL_FILES = \
numIyyy.h numRyyy.h numDyyy.h \
$(H_TMPL_FILES_XXX) $(H_TMPL_FILES_tmpl)

C_TMPL_FILES_XXX = boundXXX.c itvXXX.c eitvXXX.c
C_TMPL_FILES_tmpl = num_internal.c.tmpl num_conv.c.tmpl 
C_TMPL_FILES = \
numIyyy.c numRyyy.c numDyyy.c \
$(C_TMPL_FILES_XXX) $(C_TMPL_FILES_tmpl)

H_ORIG_FILES = \
num_internal.h \
numMPZ.h numMPFR.h \
num_all.h bound_all.h itv_all.h eitv_all.h

C_ORIG_FILES = numMPZ.c numMPQ.c numMPFR.c 

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
numIl.h numIll.h numRl.h numRll.h numD.h numDl.h \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(H_TMPL_FILES_XXX))) \
$(patsubst %.tmpl,%,$(H_TMPL_FILES_tmpl))
C_GENERATED_FILES = \
numIl.c numIll.c numRl.c numRll.c numD.c numDl.c \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_XXX))) \
$(patsubst %.tmpl,%,$(C_TMPL_FILES_tmpl))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

C_TMPL_FILES_TEST = 
C_GENERATED_FILES_TEST = $(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_TEST)))

CCINC_TO_INSTALL = $(H_TMPL_FILES) $(H_FILES)


.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES)

#---------------------------------------
# Rules
#---------------------------------------

all: src libnum.a 
debug: src libnum.d.a
prof: src libnum.p.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES)

clean:
	/bin/rm -f *.[ao]
	/bin/rm -fr html
distclean: clean
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL) 
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) $^ $(PREFIX)/include/apron 

uninstall:

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

numIl.h numIl.c: numIl.%: numIyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/l/g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

numIll.h numIll.c: numIll.%: numIyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/ll/g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

numRl.h numRl.c: numRl.%: numRyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/l/g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

numRll.h numRll.c: numRll.%: numRyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/ll/g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

numD.h numD.c: numD.%: numDyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy//g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

numDl.h numDl.c: numDl.%: numDyyy.%
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/l/g; s/#line \([0-9]*\) \"\(.*\)$@\"/#line \1 \"\2$<\"/g" >>$@

%.h: %.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@

define generate-file-XXX
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES_XXX) $(C_TMPL_FILES_XXX),$(eval $(call generate-file-XXX,$(T),$(F)))))

$(foreach T,$(TMPL),$(foreach F,$(C_TMPL_FILES_TEST),$(eval $(call generate-file-XXX,$(T),$(F)))))

.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES) $(C_GENERATED_FILES_TEST)

#-----------------------------------
# C part
#-----------------------------------

libnum.a: $(C_FILES:%.c=%.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@
libnum.d.a: $(C_FILES:%.c=%.d.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@
libnum.p.a: $(C_FILES:%.c=%.p.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

$(C_FILES:%.c=%.d.o): %.d.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

$(C_FILES:%.c=%.p.o): %.p.o: %.c
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# Test
#-----------------------------------
tests: src $(TMPL:%=testnb%) $(TMPL:%=testi1%) $(TMPL:%=testi2%)

out: tests
	for i in $(TMPL); do \
		./testnb$$i > out.$$i; \
		./testi1$$i >> out.$$i; \
		./testi2$$i >> out.$$i; \
	done

depend: $(C_FILES) $(H_FILES) $(C_FILES_TEST) 
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) $(C_FILES_TEST) >Makefile.depend.pattern
	cp Makefile.depend.pattern Makefile.depend
	$(SED) -e "s/.o:/.d.o:/g" Makefile.depend.pattern >>Makefile.depend
	$(SED) -e "s/.o:/.p.o:/g" Makefile.depend.pattern >>Makefile.depend
	rm Makefile.depend.pattern

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES) $(C_TMPL_FILES_TEST)
	etags $^
