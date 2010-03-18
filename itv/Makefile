
include ../Makefile.config

PREFIX = $(APRON_PREFIX)

SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I../num \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

CCMODULES_NAME = itv itv_conv eitv eitv_conv itv_linexpr itv_lincons # itv_linearize

CCMODULES_TMPL = $(CCMODULES_NAME:%=%_tmpl)

CCSRC = itvConfig.h $(CCMODULES_TMPL:%=%.h) $(CCMODULES_TMPL:%=%.c)

CCMODULES_IL = $(subst itv,itvIl,$(CCMODULES_NAME))
CCMODULES_ILL = $(subst itv,itvIll,$(CCMODULES_NAME))
CCMODULES_MPZ = $(subst itv,itvMPZ,$(CCMODULES_NAME))
CCMODULES_RL = $(subst itv,itvRl,$(CCMODULES_NAME))
CCMODULES_RLL = $(subst itv,itvRll,$(CCMODULES_NAME))
CCMODULES_MPQ = $(subst itv,itvMPQ,$(CCMODULES_NAME))
CCMODULES_D = $(subst itv,itvD,$(CCMODULES_NAME))
CCMODULES_DL = $(subst itv,itvDl,$(CCMODULES_NAME))
CCMODULES_MPFR = $(subst itv,itvMPFR,$(CCMODULES_NAME))

CCMODULES = \
$(CCMODULES_IL) \
$(CCMODULES_ILL) \
$(CCMODULES_MPZ) \
$(CCMODULES_RL) \
$(CCMODULES_RLL) \
$(CCMODULES_MPQ) \
$(CCMODULES_D) \
$(CCMODULES_DL) \
$(CCMODULES_MPFR)

CCINC_TO_INSTALL = itvConfig.h $(CCMODULES:%=%.h)

CCBIN_TO_INSTALL = 
CCLIB_TO_INSTALL = \
libitvIl.a libitvIl_debug.a \
libitvIll.a libitvIll_debug.a \
libitvRl.a libitvRl_debug.a \
libitvRll.a libitvRll_debug.a \
libitvMPZ.a libitvMPZ_debug.a \
libitvMPQ.a libitvMPQ_debug.a \
libitvD.a libitvD_debug.a \
libitvDl.a libitvDl_debug.a \
libitvMPFR.a libitvMPFR_debug.a \
libitv.a libitv_debug.a

OFILES = $(CCMODULES:%=%.o)


#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allIl allIll allMPZ allRl allRll allMPQ allD allDl allMPFR libitv.a libitv_debug.a

allIl: itvIl.sed libitvIl.a libitvIl_debug.a
allIll: itvIll.sed libitvIll.a libitvIll_debug.a
allMPZ: itvMPZ.sed libitvMPZ.a libitvMPZ_debug.a
allRl: itvRl.sed libitvRl.a libitvRl_debug.a
allRll: itvRll.sed libitvRll.a libitvRll_debug.a
allMPQ: itvMPQ.sed libitvMPQ.a libitvMPQ_debug.a
allD: itvD.sed libitvD.a libitvD_debug.a
allDl: itvDl.sed libitvDl.a libitvDl_debug.a
allMPFR: itvMPFR.sed libitvMPFR.a libitvMPFR_debug.a

tests: testIl testIll testMPZ testRl testRll testMPQ testD testDl testMPFR test2Il test2Ill test2MPZ test2Rl test2Rll test2MPQ test2D test2Dl test2MPFR 

clean:
	/bin/rm -fr *.[ao] testIl* testMP* testRl* testD* test2Il* test2MP* test2Rl* test2D*  
	/bin/rm -f $(CCMODULES:%=%.c) $(CCMODULES:%=%.h)
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

dist: $(CCSRC) Makefile COPYING README
	(cd ..; tar zcvf itv.tgz $(^:%=itv/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

itv%.sed: ../num/num%.sed Makefile
	cp $< $@
	echo "s/_ITV_/_ITV\\U$*_/;" >>$@
	echo "s/_EITV_/_EITV\\U$*_/;" >>$@
	echo "s/itv\(_\|(\|[.]h\)/itv$*\1/g;" >>$@

itv%.h: itv%.sed itv_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_tmpl.h\"" >$@
	$(SED) -f $< itv_tmpl.h >>$@
itv%.c: itv%.sed itv_tmpl.c itv%.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_tmpl.c\"" >$@
	$(SED) -f $< itv_tmpl.c >>$@
eitv%.h: itv%.sed eitv_tmpl.h itv%.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"eitv_tmpl.h\"" >$@
	$(SED) -f $< eitv_tmpl.h >>$@
eitv%.c: itv%.sed eitv_tmpl.c eitv%.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"eitv_tmpl.c\"" >$@
	$(SED) -f $< eitv_tmpl.c >>$@
itv%_linexpr.h: itv%.sed itv_linexpr_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_linexpr_tmpl.h\"" >$@
	$(SED) -f $< itv_linexpr_tmpl.h >>$@
itv%_linexpr.c: itv%.sed itv_linexpr_tmpl.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_linexpr_tmpl.c\"" >$@
	$(SED) -f $< itv_linexpr_tmpl.c >>$@
itv%_lincons.h: itv%.sed itv_lincons_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_lincons_tmpl.h\"" >$@
	$(SED) -f $< itv_lincons_tmpl.h >>$@
itv%_lincons.c: itv%.sed itv_lincons_tmpl.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_lincons_tmpl.c\"" >$@
	$(SED) -f $< itv_lincons_tmpl.c >>$@
itv%_conv.h: itv%.sed itv_conv_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_conv_tmpl.h\"" >$@
	$(SED) -f $< itv_conv_tmpl.h >>$@
itv%_conv.c: itv%.sed itv_conv_tmpl.c itv%_conv.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"itv_conv_tmpl.c\"" >$@
	$(SED) -f $< itv_conv_tmpl.c >>$@
eitv%_conv.h: itv%.sed eitv_conv_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"eitv_conv_tmpl.h\"" >$@
	$(SED) -f $< eitv_conv_tmpl.h >>$@
eitv%_conv.c: itv%.sed eitv_conv_tmpl.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"eitv_conv_tmpl.c\"" >$@
	$(SED) -f $< eitv_conv_tmpl.c >>$@

.PRECIOUS: $(CCMODULES:%=%.c) $(CCMODULES:%=%.h) \
itvIl.sed itvIll.sed itvMPZ.sed \
itvRl.sed itvRll.sed itvMPQ.sed \
itvD.sed itvDl.sed itvMPFR.sed


#-----------------------------------
# C part
#-----------------------------------

libitv.a: $(OFILES)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitv_debug.a: $(OFILES:%.o=%_debug.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitv%.a: itv%.o itv%_conv.o eitv%.o eitv%_conv.o itv%_linexpr.o itv%_lincons.o
	$(AR) rcs $@ $^
	$(RANLIB) $@
libitv%_debug.a: itv%_debug.o itv%_conv_debug.o eitv%_debug.o eitv%_conv_debug.o itv%_linexpr_debug.o itv%_lincons_debug.o
	$(AR) rcs $@ $^
	$(RANLIB) $@

itv%.o: itv%.c itv%.h eitv%.h itv%.sed itvMPQ.h itvD.h itvMPFR.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
itv%_debug.o: itv%.c itv%.h eitv%.h itv%.sed itvMPQ.h itvD.h itvMPFR.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
itv%_conv.o: itv%_conv.c itv%_conv.h itv%.h itv%.sed itvMPQ.h itvD.h itvMPFR.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
itv%_conv_debug.o: itv%_conv.c itv%_conv.h itv%.sed itvMPQ.h itvD.h itvMPFR.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
eitv%.o: eitv%.c eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
eitv%_debug.o: eitv%.c eitv%.h itv%.sed
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
eitv%_conv.o: eitv%_conv.c eitv%_conv.h eitv%.h itv%.h itv%.sed eitvMPQ.h eitvD.h eitvMPFR.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
eitv%_conv_debug.o: eitv%_conv.c eitv%_conv.h eitv%.h itv%.h itv%.sed eitvMPQ.h eitvD.h eitvMPFR.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
itv%_linexpr.o: itv%_linexpr.c itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
itv%_linexpr_debug.o: itv%_linexpr.c itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
itv%_lincons.o: itv%_lincons.c itv%_lincons.h itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
itv%_lincons_debug.o: itv%_lincons.c itv%_lincons.h itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
itv%_linearize.o: itv%_linearize.c itv%_linearize.h itv%_lincons.h itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
itv%_linearize_debug.o: itv%_linearize.c itv%_linearize.h itv%_lincons.h itv%_linexpr.h eitv%.h itv%.h itv%.sed
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

%.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
%_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#-----------------------------------
# Test
#-----------------------------------

test%.c: itv%.sed test.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"test.c\"" >$@
	$(SED) -f $< test.c >>$@
test2%.c: itv%.sed test2.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"test2.c\"" >$@
	$(SED) -f $< test2.c >>$@

test%: test%.c itv%.sed libitv%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -I../num -L. -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -o $@ $< -litv$*_debug -lgmp -lmpfr -lm

test2%: test2%.c itv%.sed libitv%_debug.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -I../num -L. -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib -o $@ $< -litv$*_debug -lgmp -lmpfr -lm

out: tests
	./testMPQ > out.MPQ
	./test2MPQ > out2.MPQ
	./testMPFR > out.MPFR
	./test2MPFR > out2.MPFR
	./testRll > out.Rll
	./test2Rll > out2.Rll
	./testD > out.D
	./test2D > out2.D

