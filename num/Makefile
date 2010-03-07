include ../Makefile.config

SEDSCRIPT_FILES = \
sedscriptIl sedscriptIll sedscriptMPZ \
sedscriptRl sedscriptRll sedscriptMPQ \
sedscriptD sedscriptDl sedscriptMPFR

H_ORIG_FILES = \
numConfig.h \
numintIl_def.h numintIll_def.h numintMPZ_def.h \
numratMPQ_def.h \
numfltD_def.h numfltDl_def.h numfltMPFR_def.h

H_TMPL_FILES = \
numrat_native_def_tmpl.h \
numint_tmpl.h numrat_tmpl.h numflt_tmpl.h \
num_numint_tmpl.h num_numrat_tmpl.h num_numflt_tmpl.h \
num_tmpl.h \
bound_def_tmpl.h bound_tmpl.h 

H_GENERATED_FILES = \
numratRl_def.h numratRll_def.h \
numintIl.h numintIll.h numintMPZ.h \
numratRl.h numratRll.h numratMPQ.h \
numfltD.h numfltDl.h numfltMPFR.h \
numIl_numint.h numIll_numint.h numMPZ_numint.h \
numRl_numrat.h numRll_numrat.h numMPQ_numrat.h \
numD_numflt.h numDl_numflt.h numMPFR_numflt.h \
numIl.h numIll.h numMPZ.h \
numRl.h numRll.h numMPQ.h \
numD.h numDl.h numMPFR.h \
boundIl_def.h boundIll_def.h boundMPZ_def.h \
boundRl_def.h boundRll_def.h boundMPQ_def.h \
boundD_def.h boundDl_def.h boundMPFR_def.h \
boundIl.h boundIll.h boundMPZ.h \
boundRl.h boundRll.h boundMPQ.h \
boundD.h boundDl.h boundMPFR.h

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)

all: $(H_FILES)

install:
	mkdir -p $(APRON_PREFIX)/include
	cp $(H_FILES) $(APRON_PREFIX)/include

distclean:
	(cd $(APRON_PREFIX)/include; /bin/rm -f $(H_FILES))

dist: $(SEDSCRIPT_FILES) $(H_ORIG_FILES) $(H_TMPL_FILES) Makefile README
	(cd ..; tar zcvf num.tgz $(^:%=num/%))

clean: 
	rm -fr testI* testR* testMP* testD* testMPFR* out.* TAGS $(H_GENERATED_FILES)

ICFLAGS = -I../apron -I$(APRON_PREFIX)/include -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(APRON_PREFIX)/lib -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib

CLIBS = -L../apron -lapron -lmpfr -lgmp -lm

tests: testIl testIll testRl testRll testMPZ testMPQ testD testDl testMPFR

test%.c: sedscript% test.c
	$(SED) -f $< test.c >$@

test%: test%.c sedscript%
	$(CC) -g $(CFLAGS) $(ICFLAGS) $(XCFLAGS) -o $@ $< $(CLIBS)

out: tests
	./testIl   > out.Il
	./testIll  > out.Ill
	./testRl   > out.Rl
	./testRll  > out.Rll
	./testMPZ  > out.MPZ
	./testMPQ  > out.MPQ
	./testD  > out.D
	./testDl > out.Dl
	./testMPFR > out.MPFR

numint%.h: sedscript% numint_tmpl.h 
	$(SED) -f $< numint_tmpl.h >$@
numratRl_def.h: sedscriptRl numrat_native_def_tmpl.h
	$(SED) -f $< numrat_native_def_tmpl.h >$@
numratRll_def.h: sedscriptRll numrat_native_def_tmpl.h
	$(SED) -f $< numrat_native_def_tmpl.h >$@
numrat%.h: sedscript% numrat_tmpl.h 
	$(SED) -f $< numrat_tmpl.h >$@
numflt%.h: sedscript% numflt_tmpl.h 
	$(SED) -f $< numflt_tmpl.h >$@
num%.h: sedscript% num_tmpl.h 
	$(SED) -f $< numint_tmpl.h >$@
num%_numint.h: sedscript% num_numint_tmpl.h 
	$(SED) -f $< num_numint_tmpl.h >$@
num%_numrat.h: sedscript% num_numrat_tmpl.h 
	$(SED) -f $< num_numrat_tmpl.h >$@
num%_numflt.h: sedscript% num_numflt_tmpl.h 
	$(SED) -f $< num_numflt_tmpl.h >$@
num%.h: sedscript% num_tmpl.h 
	$(SED) -f $< num_tmpl.h >$@
bound%.h: sedscript% bound_tmpl.h 
	$(SED) -f $< bound_tmpl.h >$@
bound%_def.h: sedscript% bound_def_tmpl.h 
	$(SED) -f $< bound_def_tmpl.h >$@

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_ORIG_FILES) $(H_TMPL_FILES)
	etags $^
	etags -a $(SEDSCRIPT_FILES)