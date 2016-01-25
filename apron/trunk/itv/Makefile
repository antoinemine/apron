include ../Makefile.config
include ../vars.mk

ICFLAGS += $(NUM_ICFLAGS) $(APRON_ICFLAGS) $(MP_ICFLAGS)
LDFLAGS += $(NUM_LIFLAGS) $(APRON_LIFLAGS) $(MP_LIFLAGS) -lapron_debug -lgmp -lmpfr -lm

#---------------------------------------
# Files
#---------------------------------------

CCMODULES = itv itv_linexpr itv_linearize
CCSRC = itv_config.h itv_fun.h $(CCMODULES:%=%.h) $(CCMODULES:%=%.c)

CCINC_TO_INSTALL = itv.h itv_fun.h itv_config.h itv_linexpr.h itv_linearize.h
CCBIN_TO_INSTALL = 
CCLIB_TO_INSTALL = \
libitvIl.a libitvIl_debug.a \
libitvIll.a libitvIll_debug.a \
libitvRl.a libitvRl_debug.a \
libitvRll.a libitvRll_debug.a \
libitvMPZ.a libitvMPZ_debug.a \
libitvMPQ.a libitvMPQ_debug.a \
libitvD.a libitvD_debug.a \
libitvMPFR.a libitvMPFR_debug.a \
libitv.a libitv_debug.a

ifneq ($(HAS_LONG_DOUBLE),)
CCLIB_TO_INSTALL := $(CCLIB_TO_INSTALL) libitvDl.a libitvDl_debug.a
endif

OFILES = $(CCMODULES:%=%Il.o) $(CCMODULES:%=%Ill.o) $(CCMODULES:%=%Rl.o) $(CCMODULES:%=%Rll.o) $(CCMODULES:%=%MPZ.o) $(CCMODULES:%=%MPQ.o) $(CCMODULES:%=%D.o) $(CCMODULES:%=%MPFR.o)

ifneq ($(HAS_LONG_DOUBLE),)
OFILES := $(OFILES) $(CCMODULES:%=%Dl.o) 
endif


#---------------------------------------
# Rules
#---------------------------------------

# Possible goals:
# depend doc install
# and the following one

all: allMPQ allRll allD allMPFR libitv.a libitv_debug.a

allMPQ: libitvMPQ.a libitvMPQ_debug.a

allRll: libitvRll.a libitvRll_debug.a

allD: libitvD.a libitvD_debug.a

allMPFR: libitvMPFR.a libitvMPFR_debug.a

tests: testMPQ testRll testD testMPFR test2MPQ test2Rll test2D test2MPFR

clean:
	/bin/rm -f *.[ao] testMPQ testRll testD testMPFR test2MPQ test2Rll test2D test2MPFR
	/bin/rm -f *.?.tex *.log *.aux *.bbl *.blg *.toc *.dvi *.ps *.pstex*

distclean: clean

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALLd) $(APRON_INCLUDE) $(APRON_LIB)
	$(INSTALL) $(CCINC_TO_INSTALL) $(APRON_INCLUDE)
	for i in $(CCLIB_TO_INSTALL); do \
		if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; \
	done
#	for i in $(CCBIN_TO_INSTALL); do \
#		if test -f $$i; then $(INSTALL) $$i $(APRON_BIN); fi; \
#	done

uninstall:
	for i in $(CCINC_TO_INSTALL); do /bin/rm -f $(APRON_INCLUDE)/$$i; done
	for i in $(CCLIB_TO_INSTALL); do /bin/rm -f $(APRON_LIB)/$$i; done
	for i in $(CCBIN_TO_INSTALL); do /bin/rm -f $(APRON_BIN)/$$i; done
	/bin/rm -f Makefile.depend

dist: $(CCSRC) Makefile COPYING README
	(cd ..; tar zcvf itv.tgz $(^:%=itv/%))

#---------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#---------------------------------------

.SUFFIXES: .tex .c .h .a .o

#-----------------------------------
# C part
#-----------------------------------

libitv.a: $(OFILES)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitv_debug.a: $(OFILES:%.o=%_debug.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libitv%.a: $(subst .c,%.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@
libitv%_debug.a: $(subst .c,%_debug.o,$(CCMODULES:%=%.c))
	$(AR) rcs $@ $^
	$(RANLIB) $@

%Il.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGINT -c -o $@ $<
%Il_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGINT -c -o $@ $<

%Ill.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGINT -c -o $@ $<
%Ill_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGINT -c -o $@ $<

%Rl.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<
%Rl_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGRAT -c -o $@ $<

%Rll.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<
%Rll_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGLONGRAT -c -o $@ $<

%MPZ.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPZ -c -o $@ $<
%MPZ_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPZ -c -o $@ $<

%MPQ.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<
%MPQ_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -c -o $@ $<


%D.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<
%D_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_DOUBLE -c -o $@ $<

%Dl.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<
%Dl_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGDOUBLE -c -o $@ $<

%MPFR.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<
%MPFR_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPFR -c -o $@ $<

testMPQ: testMPQ_debug.o libitvMPQ_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvMPQ_debug $(LDFLAGS)

testRll: testRll_debug.o libitvRll_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvRll_debug $(LDFLAGS)

testD: testD_debug.o libitvD_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvD_debug $(LDFLAGS)

testMPFR: testMPFR_debug.o libitvMPFR_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvMPFR_debug $(LDFLAGS)

test2MPQ: test2MPQ_debug.o libitvMPQ_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvMPQ_debug $(LDFLAGS)

test2Rll: test2Rll_debug.o libitvRll_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvRll_debug $(LDFLAGS)

test2D: test2D_debug.o libitvD_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvD_debug $(LDFLAGS)

test2MPFR: test2MPFR_debug.o libitvMPFR_debug.a
	$(CC) $(CFLAGS_DEBUG) -o $@ $< -L. -litvMPFR_debug $(LDFLAGS)

out: tests
	./testMPQ > out.MPQ
	./test2MPQ > out2.MPQ
	./testMPFR > out.MPFR
	./test2MPFR > out2.MPFR
	./testRll > out.Rll
	./test2Rll > out2.Rll
	./testD > out.D
	./test2D > out2.D

#-----------------------------------
# DEPENDENCIES
#-----------------------------------

itv%.o: itv.c itv.h itv_fun.h
itv_linexpr%.o: itv_linexpr.c itv_linexpr.h itv.h itv_fun.h
