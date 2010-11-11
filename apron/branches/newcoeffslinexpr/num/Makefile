include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib
CLIBS = -lmpfr -lgmp -lm

#---------------------------------------
# Files
#---------------------------------------

TMPL = $(APRON_NUMTYPES)

H_TMPL_FILES = \
num_types.h.tmpl \
numIyyy.h numRyyy.h numDyyy.h \
boundXXX.h \
itvXXX.h eitvXXX.h \
numXXX_conv.h

C_TMPL_FILES = \
num_internal.c.tmpl itvXXX.c eitvXXX.c \
numXXX_conv.c

H_ORIG_FILES = \
num_internal.h \
numMPZ.h numMPQ.h numMPFR.h \
num_all.h bound_all.h itv_all.h eitv_all.h

C_ORIG_FILES =

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
num_types.h \
numIl.h numIll.h \
numRl.h numRll.h \
numD.h numDl.h \
$(TMPL:%=bound%.h) \
$(TMPL:%=itv%.h) $(TMPL:%=eitv%.h) \
$(TMPL:%=num%_conv.h)
C_GENERATED_FILES = \
num_internal.c $(TMPL:%=itv%.c) $(TMPL:%=eitv%.c) \
$(TMPL:%=num%_conv.c)

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(H_TMPL_FILES) $(H_FILES)
CCBIN_TO_INSTALL = 
CCLIB_TO_INSTALL = libitv.a libitv_debug.a

.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES)

#---------------------------------------
# Rules
#---------------------------------------

all: src libitv.a libitv_debug.a
src: $(H_GENERATED_FILES) $(C_GENERATED_FILES)

clean: 
	/bin/rm -fr test*I* test*R* test*MP* test*D* test*MPFR* out.* 
	/bin/rm -f *.[ao] TAGS $(H_GENERATED_FILES) $(C_GENERATED_FILES)

mostlyclean: clean
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALLd) $(PREFIX)/include $(PREFIX)/lib
	$(INSTALL) $(CCINC_TO_INSTALL) $(PREFIX)/include
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/lib; fi; \
	done

distclean:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(PREFIX)/include/$$i; done
	for i in $(CCLIB_TO_INSTALL); do /bin/rm -f $(PREFIX)/lib/$$i; done
	for i in $(CCBIN_TO_INSTALL); do /bin/rm -f $(PREFIX)/bin/$$i; done

dist: $(H_SRC_FILES) $(C_SRC_FILES) macros.pl Makefile README
	(cd ..; tar zcvf num.tgz $(^:%=num/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

num_types.h: num_types.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< >>$@
num_internal.c: num_internal.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< >>$@

numIl.h: numIyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numIll.h: numIyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numRl.h: numRyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numRll.h: numRyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numD.h: numDyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy//g" $< >>$@

numDl.h: numDyyy.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

$(TMPL:%=bound%.h): bound%.h: boundXXX.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_types.h): itv%_types.h: itvXXX_types.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_types.c): itv%_types.c: itvXXX_types.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%.h): itv%.h: itvXXX.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=itv%.c): itv%.c: itvXXX.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=eitv%.h): eitv%.h: eitvXXX.h
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=eitv%.c): eitv%.c: eitvXXX.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=num%_conv.h): num%_conv.h: numXXX_conv.h
	@echo "/* GENERATED, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=num%_conv.c): num%_conv.c: numXXX_conv.c
	@echo "/* GENERATED, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=testnb%.c): testnb%.c: testnbXXX.c
	@echo "/* GENERATED, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@
$(TMPL:%=testi1%.c): testi1%.c: testi1XXX.c
	@echo "/* GENERATED, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@
$(TMPL:%=testi2%.c): testi2%.c: testi2XXX.c
	@echo "/* GENERATED, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES)
.PRECIOUS: $(C_GENERATED_FILES:%.c=%.o) $(C_GENERATED_FILES:%.c=%_debug.o) 
.PRECIOUS: $(TMPL:%=testnb%.c) $(TMPL:%=testi1%.c) $(TMPL:%=testi2%.c)

#-----------------------------------
# C part
#-----------------------------------

libitv.a: $(C_FILES:%.c=%.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@
libitv_debug.a: $(C_FILES:%.c=%_debug.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

$(C_FILES:%.c=%_debug.o): %_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<



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

$(TMPL:%=testnb%) $(TMPL:%=testi1%) $(TMPL:%=testi2%): %: %.c libitv_debug.a
	$(CC) -g $(CFLAGS_DEBUG) $(ICFLAGS) -o $@ $< -L. -litv_debug $(CLIBS)

depend: $(C_FILES) $(TMPL:%=testnb%.c) $(TMPL:%=testi1%.c) $(TMPL:%=testi2%.c) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES) testnbXXX.c testi1XXX.c testi2XXX.c
	etags $^
	etags -a macros.pl