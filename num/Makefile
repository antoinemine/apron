include ../Makefile.config

SRCDIR=$(shell pwd)

TMPL = Il Ill MPZ Rl Rll MPQ D Dl MPFR

H_TMPL_FILES = \
numIyyy.h numRyyy.h numDyyy.h \
boundXXX.h boundXXX_conv.h

H_ORIG_FILES = \
numConfig.h num_types.h \
numMPZ.h numMPQ.h numMPFR.h \
bound_types.h

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)

H_GENERATED_FILES = \
numIl.h numIll.h \
numRl.h numRll.h \
numD.h numDl.h \
$(TMPL:%=bound%.h) $(TMPL:%=bound%_conv.h)

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)

.PRECIOUS: $(H_GENERATED_FILES)

all: $(H_FILES)

install:
	mkdir -p $(APRON_PREFIX)/include
	cp $(H_FILES) $(APRON_PREFIX)/include

distclean:
	(cd $(APRON_PREFIX)/include; /bin/rm -f $(H_FILES))

dist: $(H_SRC_FILES) Makefile README
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

$(TMPL:%=test%.c): test%.c: testXXX.c
	echo "/* GENERATED, DO NOT MODIFY */" >$@
	echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	$(SED) -e "s/XXX/$*/g" $< >>$@

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES)
	etags $^
	etags -a macros.pl