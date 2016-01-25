include ../Makefile.config
include ../vars.mk

H_FILES = \
num_config.h \
num.h \
num_numint.h \
num_numrat.h \
num_numflt.h \
numint.h \
numint_native.h \
numint_mpz.h \
numrat.h \
numrat_native.h \
numrat_mpq.h \
numflt.h \
numflt_native.h \
numflt_mpfr.h \
num_name.h \
bound.h \
bound_def.h

all:

install:
	mkdir -p $(APRON_INCLUDE)
	cp $(H_FILES) $(APRON_INCLUDE)

uninstall:
	(cd $(APRON_INCLUDE) && /bin/rm -f $(H_FILES) || exit 0)

dist: $(H_FILES) Makefile README
	(cd ..; tar zcvf num.tgz $(^:%=num/%))

clean:
	rm -f testl* testmp* testdbl testldbl testmpfr

distclean: clean

ICFLAGS += $(APRON_ICFLAGS) $(MP_ICFLAGS)
LDFLAGS += $(APRON_LIFLAGS) $(MP_LIFLAGS) -lapron -lmpfr -lgmp -lm

tests: testli testlli testlr testllr testmpz testmpq testdbl testldbl testmpfr

testli: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_LONGINT -o $@ test.c $(LDFLAGS)

testlli: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGINT -o $@ test.c $(LDFLAGS)

testlr: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_LONGRAT -o $@ test.c $(LDFLAGS)

testllr: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_LONGLONGRAT -o $@ test.c $(LDFLAGS)

testmpz: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_MPZ -o $@ test.c $(LDFLAGS)

testmpq: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -o $@ test.c $(LDFLAGS)

testdbl: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -o $@ test.c $(LDFLAGS)

testldbl: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -o $@ test.c $(LDFLAGS)

testmpfr: test.c $(H_FILES)
	$(CC) -g $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -o $@ test.c $(LDFLAGS)

out: tests
	./testli   > out.li
	./testlli  > out.lli
	./testlr   > out.lr
	./testllr  > out.llr
	./testmpz  > out.mpz
	./testmpq  > out.mpq
	./testdbl  > out.dbl
	./testldbl > out.ldbl
	./testmpfr > out.mpfr
