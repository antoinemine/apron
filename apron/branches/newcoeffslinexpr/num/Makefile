include ../Makefile.config

SRCDIR=$(shell pwd)

TMPL = Il Ill MPZ Rl Rll MPQ D Dl MPFR

H_ORIG_FILES = \
numConfig.h numTypes.h \
numIl.h numIll.h numMPZ.h \
numMPQ.h \
numD.h numDl.h numMPFR.h \
boundTypes.h

H_TMPL_FILES = \
numRyyy.h boundXXX.h

H_GENERATED_FILES = \
numRl.h numRll.h \
$(TMPL:%=bound%.h)

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)

.PRECIOUS: $(H_GENERATED_FILES)

all: $(H_FILES)

install:
	mkdir -p $(APRON_PREFIX)/include
	cp $(H_FILES) $(APRON_PREFIX)/include

distclean:
	(cd $(APRON_PREFIX)/include; /bin/rm -f $(H_FILES))

dist: $(H_ORIG_FILES) $(H_TMPL_FILES) Makefile README
	(cd ..; tar zcvf num.tgz $(^:%=num/%))

clean: 
	rm -fr testI* testR* testMP* testD* testMPFR* out.* TAGS $(H_GENERATED_FILES)

ICFLAGS = -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include -L$(GMP_PREFIX)/lib -L$(MPFR_PREFIX)/lib

CLIBS = -lmpfr -lgmp -lm

tests: $(TMPL:%=test%)

$(TMPL:%=test%): test%: test%.c $(H_FILES)
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

numRl.h: numRyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/l/g; s/YYY/L/g" $< >>$@

numRll.h: numRyyy.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/yyy/ll/g; s/YYY/LL/g" $< >>$@

$(TMPL:%=bound%.h): bound%.h: boundXXX.h
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

$(TMPL:%=test%.c): test%.c: testXXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_ORIG_FILES) $(H_TMPL_FILES)
	etags $^
	etags -a $(SEDSCRIPT_FILES)