
include ../Makefile.config

PREFIX = $(APRON_PREFIX)

SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I../num \
-I../itv \
-I../apron \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

TMPL = Il Ill MPZ Rl Rll MPQ D Dl MPFR

H_TMPL_FILES = \
itvXXX_types.h \
itvXXX.h eitvXXX.h \
itvXXX_linexpr.h itvXXX_lincons.h \
itvXXX_conv.h
C_TMPL_FILES = \
itvXXX.c eitvXXX.c \
itvXXX_linexpr.c itvXXX_lincons.c itvXXX_YYY_array.c \
itvXXX_conv.c 

H_ORIG_FILES = itvConfig.h
C_ORIG_FILES = 

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(TMPL:%=itv%_types.h) \
$(TMPL:%=itv%.h) $(TMPL:%=eitv%.h) \
$(TMPL:%=itv%_linexpr.h) $(TMPL:%=itv%_lincons.h) \
$(TMPL:%=itv%_conv.h)
C_GENERATED_FILES = \
$(TMPL:%=itv%.c) $(TMPL:%=eitv%.c) \
$(TMPL:%=itv%_linexpr.c) $(TMPL:%=itv%_lincons.c) \
$(TMPL:%=itv%_linexpr_array.c) $(TMPL:%=itv%_lincons_array.c) \
$(TMPL:%=itv%_conv.c)

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(H_TMPL_FILES) $(H_FILES)

CCBIN_TO_INSTALL = 
CCLIB_TO_INSTALL = libitv.a libitv_debug.a

#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: src libitv.a libitv_debug.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES)
tests: testIl testIll testMPZ testRl testRll testMPQ testD testDl testMPFR test2Il test2Ill test2MPZ test2Rl test2Rll test2MPQ test2D test2Dl test2MPFR 

clean:
	/bin/rm -fr *.[ao] testIl* testMP* testRl* testD* test2Il* test2MP* test2Rl* test2D*  
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*

mostlyclean: clean

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALLd) $(PREFIX)/include $(PREFIX)/lib
	$(INSTALL) $(CCINC_TO_INSTALL) $(PREFIX)/include
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/lib; fi; \
	done
#	for i in $(CCBIN_TO_INSTALL); do \
#		if test -f $$i; then $(INSTALL) $$i $(PREFIX)/bin; fi; \
#	done

distclean:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(PREFIX)/include/$$i; done
	for i in $(CCLIB_TO_INSTALL); do /bin/rm -f $(PREFIX)/lib/$$i; done
	for i in $(CCBIN_TO_INSTALL); do /bin/rm -f $(PREFIX)/bin/$$i; done
	/bin/rm -f Makefile.depend

dist: $(H_SRC_FILES) $(C_SRC_FILES) macros.pl Makefile COPYING README
	(cd ..; tar zcvf itv.tgz $(^:%=itv/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

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

$(TMPL:%=itv%_linexpr.h): itv%_linexpr.h: itvXXX_linexpr.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@
$(TMPL:%=itv%_linexpr.h): itv%_linexpr.h: ../apron/ap_coeff.h
../apron/ap_coeff.h: ../apron/ap_coeff.h.tmpl
	../apron/macros.pl $< >$@

$(TMPL:%=itv%_linexpr.c): itv%_linexpr.c: itvXXX_linexpr.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_lincons.h): itv%_lincons.h: itvXXX_lincons.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_lincons.c): itv%_lincons.c: itvXXX_lincons.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=itv%_linexpr_array.c): itv%_linexpr_array.c: itvXXX_YYY_array.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g; s/YYY/linexpr/g" $< >>$@

$(TMPL:%=itv%_lincons_array.c): itv%_lincons_array.c: itvXXX_YYY_array.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g; s/YYY/lincons/g" $< >>$@

$(TMPL:%=itv%_conv.h): itv%_conv.h: itvXXX_conv.h
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g;s/$@/$</g" >>$@

$(TMPL:%=itv%_conv.c): itv%_conv.c: itvXXX_conv.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g;s/$@/$</g" >>$@

.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES)
.PRECIOUS: $(C_GENERATED_FILES:%.c=%.o) $(C_GENERATED_FILES:%.c=%_debug.o) 
.PRECIOUS: $(TMPL:%=test%.c) $(TMPL:%=test2%.c)

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

$(TMPL:%=test%.c): test%.c: testXXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=test2%.c): test2%.c: test2XXX.c
	echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=test%): test%: test%.c libitv_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -I../num -L. -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -o $@ $< -litv_debug -lgmp -lmpfr -lm

$(TMPL:%=test2%): test2%: test2%.c libitv_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -I../num -L. -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -o $@ $< -litv_debug -lgmp -lmpfr -lm

out: tests
	./testMPQ > out.MPQ
	./test2MPQ > out2.MPQ
	./testMPFR > out.MPFR
	./test2MPFR > out2.MPFR
	./testRll > out.Rll
	./test2Rll > out2.Rll
	./testD > out.D
	./test2D > out2.D

depend: $(C_FILES) $(TMPL:%=test%.c) $(TMPL:%=test2%.c) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES) testXXX.c test2XXX.c
	etags $^
	etags -a macros.pl
