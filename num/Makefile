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
boundXXX.h boundXXX_conv.h \
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
$(TMPL:%=bound%.h) $(TMPL:%=bound%_conv.h) \
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

tests: $(TMPL:%=testnb%) $(TMPL:%=testitv1%) $(TMPL:%=testitv2%)

out: tests
	for i in $(TMPL); do \
		./testnb$$i > out.$$i; \
		./testi1$$i >> out.$$i; \
		./testi2$$i >> out.$$i; \
	done

$(TMPL:%=testnb%): testnb%: testnb%.c libitv_debug.a
	$(CC) -g $(CFLAGS) $(ICFLAGS) $(XCFLAGS) -o $@ $< -litv_debug $(CLIBS)
$(TMPL:%=testi1%): testi1%: testi1%.c libitv_debug.a
	$(CC) -g $(CFLAGS) $(ICFLAGS) $(XCFLAGS) -o $@ $< -litv_debug $(CLIBS)
$(TMPL:%=testi2%): testi2%: testi2%.c libitv_debug.a
	$(CC) -g $(CFLAGS) $(ICFLAGS) $(XCFLAGS) -o $@ $< -litv_debug $(CLIBS)

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

numIl.h: numIyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numIll.h: numIyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numRl.h: numRyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

numRll.h: numRyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g" $< >>$@

numD.h: numDyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy//g" $< >>$@

numDl.h: numDyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g" $< >>$@

$(TMPL:%=bound%.h): bound%.h: boundXXX.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=bound%_conv.h): bound%_conv.h: boundXXX_conv.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	macros.pl $< | $(SED) -e "s/XXX/$*/g" >>$@

$(TMPL:%=itv%_types.h): itv%_types.h: itvXXX_types.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%.h): itv%.h: itvXXX.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%.c): itv%.c: itvXXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=eitv%.h): eitv%.h: eitvXXX.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=eitv%.c): eitv%.c: eitvXXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=test%.c): test%.c: testXXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl