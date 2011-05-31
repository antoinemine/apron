include ../Makefile.config
SRCDIR = $(shell pwd)
ICFLAGS = \
-I$(MPFR_PREFIX)/include -I$(GMP_PREFIX)/include \
-I../num -I../apron -I../box -I../polka

LCFLAGS = \
-L$(MPFR_PREFIX)/lib -L$(GMP_PREFIX)/lib \
-L../num -L../apron -L../box -L../polka
# -lpolka.d -lbox.d -lapron.d -lnum.d -lmpfr -lgmp

C_TMPL_FILES_tmpl = num.c.tmpl domain.c.tmpl
C_GENERATED_FILES = $(patsubst %.tmpl,%,$(C_TMPL_FILES_tmpl))
C_SRC_FILES = $(C_TMPL_FILES)
C_FILES = $(C_GENERATED_FILES)

all: num

# C examples

num: num.c ../num/libnum.d.a
	$(CC) $(CFLAGS.D) $(ICFLAGS) $(LCFLAGS) -o $@ $^ -lnum.d -lmpfr -lgmp -lm
domain: domain.c ../num/libnum.d.a ../apron/libapron.d.a ../box/libbox.d.a ../polka/libpolka.d.a
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) $(LCFLAGS) -o $@ $^ -lpolka.d -lbox.d -lapron.d -lnum.d -lmpfr -lgmp -lm

clean:
	rm -f num domain *.o out/*

mostlyclean: clean

distclean: mostlyclean
	rm -f $(C_GENERATED_FILES)

%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@
