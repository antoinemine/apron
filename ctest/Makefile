include ../Makefile.config
SRCDIR = $(shell pwd)
ICFLAGS = \
-I$(MPFR_PREFIX)/include -I$(GMP_PREFIX)/include \
-I../apron -I../box -I../polka

LCFLAGS = \
-L$(MPFR_PREFIX)/lib -L$(GMP_PREFIX)/lib \
-L../apron -L../box -L../polka
# -lpolka.d -lbox.d -lapron.d -lnum.d -lmpfr -lgmp

C_TMPL_FILES_tmpl = num.c.tmpl domain.c.tmpl
C_GENERATED_FILES = $(patsubst %.tmpl,%,$(C_TMPL_FILES_tmpl))
C_SRC_FILES = $(C_TMPL_FILES)
C_FILES = $(C_GENERATED_FILES)

all: num

# C examples

num: num.c ../apron/libapron.d.a 
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) $(LCFLAGS) -o $@ $< -lapron.d -lmpfr -lgmp -lm
expr: expr.c ../apron/libapron.d.a 
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) $(LCFLAGS) -o $@ $< -lapron.d -lmpfr -lgmp -lm
domain: domain.c ../apron/libapron.p.a ../box/libbox.p.a ../polka/libpolka.p.a
	$(CC) $(CFLAGS_PROF) -g3 -pg $(ICFLAGS) $(LCFLAGS) -o $@ $< -lpolka.p -lbox.p -lapron.p -lmpfr -lgmp -lm

clean:
	rm -f num expr domain *.o out/*

mostlyclean: clean

distclean: mostlyclean
	rm -f $(C_GENERATED_FILES)

%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@
