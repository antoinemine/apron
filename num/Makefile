include ../Makefile.config

SEDSCRIPT_FILES = \
numIl.sed numIll.sed numMPZ.sed \
numRl.sed numRll.sed numMPQ.sed \
numD.sed numDl.sed numMPFR.sed

H_ORIG_FILES = \
numConfig.h \
numintIl_def.h numintIll_def.h numintMPZ_def.h \
numratMPQ_def.h \
numfltD_def.h numfltDl_def.h numfltMPFR_def.h

H_TMPL_FILES = \
numrat_native_def_tmpl.h \
numint_tmpl.h numrat_tmpl.h numflt_tmpl.h \
num_tmpl.h num_def_tmpl.h num_conv_tmpl.h \
bound_def_tmpl.h bound_tmpl.h bound_conv_tmpl.h

H_GENERATED_FILES = \
numratRl_def.h numratRll_def.h \
numintIl.h numintIll.h numintMPZ.h \
numratRl.h numratRll.h numratMPQ.h \
numfltD.h numfltDl.h numfltMPFR.h \
numIl_def.h numIll_def.h numMPZ_def.h \
numRl_def.h numRll_def.h numMPQ_def.h \
numD_def.h numDl_def.h numMPFR_def.h \
numIl.h numIll.h numMPZ.h \
numRl.h numRll.h numMPQ.h \
numD.h numDl.h numMPFR.h \
numIl_conv.h numIll_conv.h numMPZ_conv.h \
numRl_conv.h numRll_conv.h numMPQ_conv.h \
numD_conv.h numDl_conv.h numMPFR_conv.h \
boundIl_def.h boundIll_def.h boundMPZ_def.h \
boundRl_def.h boundRll_def.h boundMPQ_def.h \
boundD_def.h boundDl_def.h boundMPFR_def.h \
boundIl.h boundIll.h boundMPZ.h \
boundRl.h boundRll.h boundMPQ.h \
boundD.h boundDl.h boundMPFR.h \
boundIl_conv.h boundIll_conv.h boundMPZ_conv.h \
boundRl_conv.h boundRll_conv.h boundMPQ_conv.h \
boundD_conv.h boundDl_conv.h boundMPFR_conv.h 

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)

.PRECIOUS: $(H_GENERATED_FILES)

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

ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib

CLIBS = -lmpfr -lgmp -lm

tests: testIl testIll testRl testRll testMPZ testMPQ testD testDl testMPFR

test%.c: num%.sed test.c
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/test.c\"" >$@
	$(SED) -f $< test.c >>$@

test%: test%.c num%.sed $(H_FILES)
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

numint%.h: num%.sed numint_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/numint_tmpl.h\"" >$@
	$(SED) -f $< numint_tmpl.h >>$@
numratRl_def.h: numRl.sed numrat_native_def_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/numrat_native_def_tmpl.h\"" >$@
	$(SED) -f $< numrat_native_def_tmpl.h >>$@
numratRll_def.h: numRll.sed numrat_native_def_tmpl.h
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/numrat_native_def_tmpl.h\"" >$@
	$(SED) -f $< numrat_native_def_tmpl.h >>$@
numrat%.h: num%.sed numrat_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/numrat_tmpl.h\"" >$@
	$(SED) -f $< numrat_tmpl.h >>$@
numflt%.h: num%.sed numflt_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/numflt_tmpl.h\"" >$@
	$(SED) -f $< numflt_tmpl.h >>$@
num%_def.h: num%.sed num_def_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/num_def_tmpl.h\"" >$@
	$(SED) -f $< num_def_tmpl.h >>$@
num%.h: num%.sed num_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/num_tmpl.h\"" >$@
	$(SED) -f $< num_tmpl.h >>$@
num%_conv.h: num%.sed num_conv_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/num_conv_tmpl.h\"" >$@
	$(SED) -f $< num_conv_tmpl.h >>$@
bound%.h: num%.sed bound_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/bound_tmpl.h\"" >$@
	$(SED) -f $< bound_tmpl.h >>$@
bound%_def.h: num%.sed bound_def_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/bound_def_tmpl.h\"" >$@
	$(SED) -f $< bound_def_tmpl.h >>$@
bound%_conv.h: num%.sed bound_conv_tmpl.h 
	echo "/* GENERATED, DO NOT MODIFY */\n#line 1 \"$(SRCDIR)/bound_conv_tmpl.h\"" >$@
	$(SED) -f $< bound_conv_tmpl.h >>$@

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_ORIG_FILES) $(H_TMPL_FILES)
	etags $^
	etags -a $(SEDSCRIPT_FILES)