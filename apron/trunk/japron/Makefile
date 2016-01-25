# Makefile
#
# APRON Library / Java Binding
#
# Copyright (C) Antoine Mine' 2010

# This file is part of the APRON Library, released under LGPL license
# with an exception allowing the redistribution of statically linked
# executables.

# Please read the COPYING file packaged in the distribution.

include ../Makefile.config
include ../vars.mk

############################

IFLAGS = $(ICFLAGS) \
-I$(GMP_PREFIX)/include \
-I$(MPFR_PREFIX)/include \
-I$(PPL_PREFIX)/include \
-I../apron \
-I../box \
-I../newpolka \
-I../octagons \
-I../ppl \
-I../products \
$(JNIINC) 

LFLAGS = $(LDFLAGS) \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(PPL_PREFIX)/lib \
-L. \
-L../apron \
-L../box \
-L../newpolka \
-L../octagons \
-L../ppl \
-L../products


CFLAGSN = $(CFLAGS) -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-bad-function-cast

############################
#SOURCES
########

GMPLIBS   = -lmpfr -lgmp -lm

# .java / .c pairs
GMPMODS = Mpz MpzRef Mpq Mpfr RandState

# lone .java
GMPJAVA = Test

APRONLIBS = -ljgmp -lboxD -loctD -lpolkaMPQ -lapron $(GMPLIBS)

# .java / .c pairs
APRONMODS = Dimchange Dimperm Linexpr0 Lincons0 \
            Texpr0Intern Manager Abstract0 Environment \
	    Octagon Polka PolkaEq Box

ifneq ($(HAS_PPL),)
APRONMODS := $(APRONMODS) PplPoly PplGrid PolkaGrid
APRONLIBS := $(APRONLIBS) -lap_pkgrid -lap_ppl -lppl
endif

# long .java
APRONJAVA = Scalar MpqScalar MpfrScalar DoubleScalar Interval Coeff \
            Linterm0 Dimension Texpr0Node Texpr0BinNode Texpr0UnNode \
            Texpr0DimNode Texpr0CstNode Tcons0 Generator0 Linterm1 Linexpr1 \
            Lincons1 Texpr1Intern Texpr1Node Texpr1BinNode Texpr1UnNode \
            Texpr1VarNode Texpr1CstNode Tcons1 Generator1 Abstract1 \
            ApronException NotImplementedException OutOfSpaceException \
            OverflowException TimeoutException \
            Test

############################
#OBJECTS
########

GMPJ       = $(addprefix gmp/,$(addsuffix .java,$(GMPMODS) $(GMPJAVA)))
GMPCLASS   = $(subst .java,.class,$(GMPJ))
GMPC       = $(addprefix gmp/gmp_,$(addsuffix .c,$(GMPMODS)))
GMPO       = $(addprefix gmp/gmp_,$(addsuffix .o,$(GMPMODS)))
GMPH       = $(addprefix gmp/gmp_,$(addsuffix .h,$(GMPMODS)))

APRONJ     = $(addprefix apron/,$(addsuffix .java,$(APRONMODS) $(APRONJAVA)))
APRONCLASS = $(subst .java,.class,$(APRONJ))
APRONC     = $(addprefix apron/apron_,$(addsuffix .c,$(APRONMODS)))
APRONO     = $(addprefix apron/apron_,$(addsuffix .o,$(APRONMODS)))
APRONH     = $(addprefix apron/apron_,$(addsuffix .h,$(APRONMODS)))

GMPALL     = $(GMPCLASS) $(GMPH) libjgmp.so
APRONALL   = $(APRONCLASS) $(APRONH) $(APRONO) libjapron.so


# built and installed in JAVA_PREFIX
JAVAINST   = gmp.jar apron.jar

# built and installed in APRON_LIB
SOINST     = libjgmp.so libjapron.so

############################
#TARGETS
########

all: $(GMPALL) $(APRONALL) $(JAVAINST) $(SOINST)

$(GMPCLASS): $(GMPJ)
	$(JAVAC) $+

gmp/%.o: gmp/%.c $(GMPH) gmp/jgmp.h
	$(CC) $(CFLAGSN) -c $(IFLAGS) $< -o $@

libjgmp.so: gmp/jgmp.o $(GMPO)
	$(CC) $(CFLAGSN) -shared $+ -o $@ $(LFLAGS) $(GMPLIBS)

gmp/gmp_%.h: gmp/%.java
	$(JAVAH) -o $@ gmp.$* 
	touch $@

gmp.jar: $(GMPCLASS)
	$(JAR) cf $@ $+
	$(JAR) i $@


$(APRONCLASS): $(APRONJ)
	$(JAVAC) $+

apron/%.o: apron/%.c $(APRONH)  gmp/jgmp.h apron/japron.h
	$(CC) $(CFLAGSN) -c $(IFLAGS) -Igmp $< -o $@

libjapron.so: apron/japron.o $(APRONO)
	$(CC) $(CFLAGSN) -shared $+ -o $@ $(LFLAGS) $(APRONLIBS)

apron/apron_%.h: apron/%.java
	$(JAVAH) -o $@ apron.$* 
	touch $@

apron.jar: $(APRONCLASS)
	$(JAR) cf $@ $+
	$(JAR) i $@

doc:
	$(JAVADOC) -d doc gmp apron

install:
	$(INSTALLd) $(APRON_LIB); \
	for i in $(SOINST); do \
		$(INSTALL) $$i $(APRON_LIB); \
	done; \
	$(INSTALLd) $(JAVA_PREFIX); \
	for i in $(JAVAINST); do \
		$(INSTALL) $$i $(JAVA_PREFIX); \
	done

uninstall:
	for i in $(SOINST); do \
		/bin/rm -f $(APRON_LIB)/$$i; \
	done; \
	for i in $(JAVAINST); do \
		/bin/rm -f $(JAVA_PREFIX)/$$i; \
	done

distclean: clean

test:
	$(JAVA) -cp gmp.jar -ea -esa -Xcheck:jni gmp.Test
	$(JAVA) -cp gmp.jar:apron.jar -ea -esa apron.Test

dist: $(GMPJ) $(APRONJ) $(GMPC) $(APRONC) apron/japron.c apron/japron.h gmp/jgmp.c gmp/jgmp.h Makefile README COPYING
	(cd ..; tar zcvf japron.tgz $(^:%=japron/%))


clean:
	rm -f *~ \#* */*~ *.o *.class *.so *.jar */\#* */*.o */*.class */*.so $(GMPH) $(APRONH)
	rm -rf doc

.PHONY: clean doc install unsintall distclean dist

