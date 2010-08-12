include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include
CLIBS = -lmpfr -lgmp -lm

#---------------------------------------
# Files
#---------------------------------------

TMPL = Il Ill MPZ Rl Rll MPQ D Dl MPFR

H_TMPL_FILES = \
numIyyy.h numRyyy.h numDyyy.h \
boundXXX.h \
itvXXX_types.h itvXXX.h eitvXXX.h \
numbounditvXXX_conv.h

C_TMPL_FILES = \
itvXXX_types.c itvXXX.c eitvXXX.c \
numbounditvXXX_conv.c

H_ORIG_FILES = \
num_types.h \
numMPZ.h numMPQ.h numMPFR.h \
bound_types.h \
itv_types.h 

C_ORIG_FILES = \
itv_types.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
numIl.h numIll.h \
numRl.h numRll.h \
numD.h numDl.h \
$(TMPL:%=bound%.h) \
$(TMPL:%=itv%_types.h) $(TMPL:%=itv%.h) $(TMPL:%=eitv%.h) \
$(TMPL:%=numbounditv%_conv.h)
C_GENERATED_FILES = \
$(TMPL:%=itv%_types.c) $(TMPL:%=itv%.c) $(TMPL:%=eitv%.c) \
$(TMPL:%=numbounditv%_conv.c)

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
	/bin/rm -fr test*I* test*R* test*MP* test*D* test*MPFR* out.* TAGS $(H_GENERATED_FILES) $(C_GENERATED_FILES)

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

ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

numIl.h: numIyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numIll.h: numIyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numRl.h: numRyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numRll.h: numRyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numD.h: numDyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy//g" $< >>$@

numDl.h: numDyyy.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

$(TMPL:%=bound%.h): bound%.h: boundXXX.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_types.h): itv%_types.h: itvXXX_types.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_types.c): itv%_types.c: itvXXX_types.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%.h): itv%.h: itvXXX.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=itv%.c): itv%.c: itvXXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=eitv%.h): eitv%.h: eitvXXX.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=eitv%.c): eitv%.c: eitvXXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=numbounditv%_conv.h): numbounditv%_conv.h: numbounditvXXX_conv.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=numbounditv%_conv.c): numbounditv%_conv.c: numbounditvXXX_conv.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

$(TMPL:%=testnb%.c): testnb%.c: testnbXXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@
$(TMPL:%=testi1%.c): testi1%.c: testi1XXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@
$(TMPL:%=testi2%.c): testi2%.c: testi2XXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
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

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# Test
#-----------------------------------
tests: $(TMPL:%=testnb%) $(TMPL:%=testitv1%) $(TMPL:%=testitv2%)

out: tests
	for i in $(TMPL); do \
		./testnb$$i > out.$$i; \
		./testi1$$i >> out.$$i; \
		./testi2$$i >> out.$$i; \
	done

$(TMPL:%=testnb%) $(TMPL:%=testi1%) $(TMPL:%=testi2%): %: %_debug.o libitv_debug.a
	$(CC) -g $(CFLAGS) $(ICFLAGS) -o $@ $< -L. -litv_debug $(CLIBS)

depend: $(C_FILES) $(TMPL:%=testnb%.c) $(TMPL:%=testi1%.c) $(TMPL:%=testi2%.c) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl