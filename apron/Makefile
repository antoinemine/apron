include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

SRCDIR = $(shell pwd)
#
PREFIX = $(APRON_PREFIX)
#
# C include and lib directories
INCDIR = $(PREFIX)/include
LIBDIR = $(PREFIX)/lib
BINDIR = $(PREFIX)/bin
#

#---------------------------------------
# C part
#---------------------------------------
ICFLAGS = -I. -I../num -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

TMPL = $(APRON_NUMTYPES)

H_TMPL_FILES = \
ap_linexprXXX.h ap_linconsXXX.h ap_lingenXXX.h \
ap_linexprconsgenXXX_conv.h \
ap_coeff.h.tmpl ap_box0.h.tmpl \
ap_linyyy0.h ap_linyyy1.h ap_linyyy1_array.h 
C_TMPL_FILES = \
ap_linexprXXX.c ap_linconsXXX.c ap_lingenXXX.c ap_linyyyXXX_array.c \
ap_linexprconsgenXXX_conv.c \
ap_coeff.c.tmpl ap_box0.c.tmpl \
ap_linyyy0.c ap_linyyy1.c ap_linyyy1_array.c \
ap_check0.c.tmpl ap_generic.c.tmpl

H_ORIG_FILES = \
ap_config.h ap_dimension.h \
ap_texpr0.h ap_tcons0.h \
ap_manager.h \
ap_var.h ap_environment.h \
ap_check0.h ap_abstract0.h \
ap_generic.h
#ap_linexpr0_array.h \
#ap_lincons0_array.h \ap_
#ap_generator0.h
#ap_texpr0.h ap_tcons0.h ap_expr0.h \
#ap_manager.h ap_abstract0.h ap_generic.h \
#ap_lincons1.h ap_generator1.h \
#ap_texpr1.h ap_tcons1.h ap_expr1.h \
#ap_abstract1.h \
#ap_global0.h ap_global1.h \
#ap_linearize.h ap_linearize_aux.h \
#ap_reducedproduct.h
C_ORIG_FILES = \
ap_dimension.c \
ap_texpr0.c ap_tcons0.c \
ap_manager.c \
ap_abstract0.c \
ap_var.c ap_environment.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(TMPL:%=ap_linexpr%.h) $(TMPL:%=ap_lincons%.h) $(TMPL:%=ap_lingen%.h) \
$(TMPL:%=ap_linexprconsgen%_conv.h) \
ap_coeff.h ap_box0.h \
ap_linexpr0.h ap_lincons0.h ap_lingen0.h \
ap_linexpr1.h ap_lincons1.h ap_lingen1.h \
ap_linexpr1_array.h ap_lincons1_array.h ap_lingen1_array.h
C_GENERATED_FILES = \
$(TMPL:%=ap_linexpr%.c) $(TMPL:%=ap_lincons%.c) $(TMPL:%=ap_lingen%.c) \
$(TMPL:%=ap_linexpr%_array.c) $(TMPL:%=ap_lincons%_array.c) $(TMPL:%=ap_lingen%_array.c) \
$(TMPL:%=ap_linexprconsgen%_conv.c) \
ap_coeff.c ap_box0.c \
ap_linexpr0.c ap_lincons0.c ap_lingen0.c \
ap_linexpr1.c ap_lincons1.c ap_lingen1.c \
ap_linexpr1_array.c ap_lincons1_array.c ap_lingen1_array.c \
ap_check0.c ap_generic.c

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(H_FILES)
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = \
$(TMPL:%=libapron%.a) $(TMPL:%=libapron%_debug.a) \
$(TMPL:%=libapron%.so) $(TMPL:%=libapron%_debug.so)

#---------------------------------------
# Rules
#---------------------------------------

all: libapron.a libapron_debug.a
ifneq ($(HAS_SHARED),)
all: libapron.so libapron_debug.so
endif

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

showsrc:
	@for i in $(H_SRC_FILES) $(C_SRC_FILES); do echo $$i; done

#---------------------------------------
# Misc rules
#---------------------------------------

clean:
	/bin/rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *.ps *.pdf apron.cps apron.fns apron.info apron.fn apron.ky apron.pg apron.cp apron.tp apron.vr apron.kys apron.pgs apron.tps apron.vrs newpolka.texi box.texi ap_ppl.texi ap_pkgrid.texi apron.info*
	/bin/rm -f *.o *.a *.cmi *.cmo *.cmx *.cmxa *.cma *.so
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -fr html
distclean: clean
	/bin/rm -f apron.pdf rationale.pdf

install: all
	$(INSTALLd) $(APRON_PREFIX)/include
	$(INSTALLd) $(APRON_PREFIX)/lib
	for i in $(CCINC_TO_INSTALL); if test -f $$i; then  $(INSTALL) $$i $(APRON_PREFIX)/include; fi done
	for i in $(CCLIB_TO_INSTALL); do if test -f $$i; then $(INSTALL) $$i $(APRON_PREFIX)/lib; fi; done

uninstall:
	/bin/rm -f $(CCINC_TO_INSTALL:%=$(APRON_PREFIX)/include/%)
	/bin/rm -f $(CCLIB_TO_INSTALL:%=$(APRON_PREFIX)/lib/%)

#---------------------------------------
# Latex rules
#---------------------------------------

.PHONY : html

ap_pkgrid.texi: ../products/ap_pkgrid.texi
	ln -s $< $@
ap_ppl.texi: ../ppl/ap_ppl.texi
	ln -s $< $@
newpolka.texi: ../newpolka/newpolka.texi
	ln -s $< $@
box.texi: ../box/box.texi
	ln -s $< $@

apron.pdf: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi newpolka.texi box.texi 
	$(TEXI2DVI) --pdf -o $@ $<

apron.info: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi newpolka.texi box.texi
	$(MAKEINFO) -o $@ $<


html: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi newpolka.texi box.texi 
	$(TEXI2HTML) -split=section -nonumber -menu -subdir=html $<
	cp -f ../octagons/oct_doc.html html

#---------------------------------------
# C rules
#---------------------------------------

libapron.a: $(C_FILES:%.c=%.o) ../num/libitv.a
	cp ../num/libitv.a $@
	$(AR) rs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libapron_debug.a: $(C_FILES:%.c=%_debug.o) ../num/libitv_debug.a
	cp ../num/libitv_debug.a $@
	$(AR) rs $@ $(C_FILES:%.c=%_debug.o)
	$(RANLIB) $@

libapron.so: $(C_FILES:%.c=%.o) ../num/libitv.a
	$(CC) $(CFLAGS) -shared -o $@ $(C_FILES:%.c=%.o) \
	$(patsubst %,../num/%,$(filter %.o, $(shell $(AR) t ../num/libitv.a))) 

libapron_debug.so: $(C_FILES:%.c=%_debug.o) ../num/libitv_debug.a
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $(C_FILES:%.c=%_debug.o) \
	$(patsubst %,../num/%,$(filter %.o, $(shell $(AR) t ../num/libitv_debug.a))) 

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# Files generation
#---------------------------------------

define generate-file-XXX
@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
./macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
endef

$(TMPL:%=ap_linexpr%.h): ap_linexpr%.h: ap_linexprXXX.h
	$(generate-file-XXX)
$(TMPL:%=ap_linexpr%.c): ap_linexpr%.c: ap_linexprXXX.c
	$(generate-file-XXX)
$(TMPL:%=ap_lincons%.h): ap_lincons%.h: ap_linconsXXX.h
	$(generate-file-XXX)
$(TMPL:%=ap_lincons%.c): ap_lincons%.c: ap_linconsXXX.c
	$(generate-file-XXX)
$(TMPL:%=ap_lingen%.h): ap_lingen%.h: ap_lingenXXX.h
	$(generate-file-XXX)
$(TMPL:%=ap_lingen%.c): ap_lingen%.c: ap_lingenXXX.c
	$(generate-file-XXX)
$(TMPL:%=ap_linexprconsgen%_conv.h): ap_linexprconsgen%_conv.h: ap_linexprconsgenXXX_conv.h
	$(generate-file-XXX)
$(TMPL:%=ap_linexprconsgen%_conv.c): ap_linexprconsgen%_conv.c: ap_linexprconsgenXXX_conv.c
	$(generate-file-XXX)

$(TMPL:%=ap_linexpr%_array.c): ap_linexpr%_array.c: ap_linyyyXXX_array.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/yyy/expr/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
$(TMPL:%=ap_lincons%_array.c): ap_lincons%_array.c: ap_linyyyXXX_array.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/yyy/cons/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
$(TMPL:%=ap_lingen%_array.c): ap_lingen%_array.c: ap_linyyyXXX_array.c
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/yyy/gen/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@

define generate-file-expr-yyy
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/expr/g;s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
endef
define generate-file-cons-yyy
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/cons/g;s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
endef
define generate-file-gen-yyy
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< | $(SED) -e "s/yyy/gen/g;s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
endef
ap_linexpr0.h ap_linexpr0.c ap_linexpr1.h ap_linexpr1.c: ap_linexpr%: ap_linyyy%
	$(generate-file-expr-yyy)
ap_linexpr0_array.h ap_linexpr0_array.c: ap_linexpr0_array.%: ap_linyyy0_array.%
	$(generate-file-expr-yyy)
ap_linexpr1_array.h ap_linexpr1_array.c: ap_linexpr1_array.%: ap_linyyy1_array.%
	$(generate-file-expr-yyy)
ap_lincons0.h ap_lincons0.c ap_lincons1.h ap_lincons1.c: ap_lincons%: ap_linyyy%
	$(generate-file-cons-yyy)
ap_lincons0_array.h ap_lincons0_array.c: ap_lincons0_array.%: ap_linyyy0_array.%
	$(generate-file-cons-yyy)
ap_lincons1_array.h ap_lincons1_array.c: ap_lincons1_array.%: ap_linyyy1_array.%
	$(generate-file-cons-yyy)
ap_lingen0.h ap_lingen0.c ap_lingen1.h ap_lingen1.c: ap_lingen%: ap_linyyy%
	$(generate-file-gen-yyy)
ap_lingen0_array.h ap_lingen0_array.c: ap_lingen0_array.%: ap_linyyy0_array.%
	$(generate-file-gen-yyy)
ap_lingen1_array.h ap_lingen1_array.c: ap_lingen1_array.%: ap_linyyy1_array.%
	$(generate-file-gen-yyy)

%.h: %.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< >>$@

#---------------------------------------
# C generic rules
#---------------------------------------

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%_debug.o): %_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

#---------------------------------------
# dependencies (generated with make depend)
#---------------------------------------

Makefile.depend depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl

