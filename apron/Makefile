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
ICFLAGS = -I../num -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

H_FILES = \
ap_config.h \
ap_scalar.h ap_interval.h ap_coeff.h ap_dimension.h \
ap_linexpr0.h ap_lincons0.h ap_generator0.h \
ap_texpr0.h ap_tcons0.h ap_expr0.h \
ap_manager.h ap_abstract0.h ap_policy.h ap_generic.h \
ap_var.h ap_environment.h \
ap_linexpr1.h ap_lincons1.h ap_generator1.h \
ap_texpr1.h ap_tcons1.h ap_expr1.h \
ap_abstract1.h \
ap_global0.h ap_global1.h \
ap_linearize.h ap_linearize_aux.h \
ap_reducedproduct.h

C_FILES = \
ap_scalar.c ap_interval.c ap_coeff.c ap_dimension.c \
ap_linexpr0.c ap_lincons0.c ap_generator0.c \
ap_texpr0.c ap_tcons0.c \
ap_manager.c ap_abstract0.c ap_policy.c ap_generic.c \
ap_var.c ap_environment.c \
ap_linexpr1.c ap_lincons1.c ap_generator1.c \
ap_texpr1.c ap_tcons1.c \
ap_abstract1.c \
ap_linearize.c \
ap_reducedproduct.c

C_FILES_AUX = ap_linearize_aux.c
H_FILES_AUX = ap_linearize_aux.h
CH_FILES_AUX = $(H_FILES_AUX) $(C_FILES_AUX)

LIBS = -lm -L$(GMP_PREFIX)/lib -lgmp -L$(MPFR_PREFIX)/lib -lmpfr

#---------------------------------------
# Rules
#---------------------------------------

all: libapron.a libapron_debug.a
ifneq ($(HAS_SHARED),)
all: libapron.so libapron_debug.so
endif

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(H_FILES) $(C_FILES) $(CH_FILES_AUX) apron.texi Makefile COPYING README
	(cd ..; tar zcvf apron.tgz $(^:%=apron/%))

dist: $(H_FILES) $(C_FILES) $(CH_FILES_AUX) apron.texi Makefile COPYING README apron.info apron.pdf html
	(cd ..; tar zcvf apron.tgz $(^:%=apron/%))

clean:
	/bin/rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *.ps *.pdf apron.cps apron.fns apron.info apron.fn apron.ky apron.pg apron.cp apron.tp apron.vr apron.kys apron.pgs apron.tps apron.vrs newpolka.texi box.texi ap_ppl.texi ap_pkgrid.texi apron.info*
	/bin/rm -f *.o *.a *.cmi *.cmo *.cmx *.cmxa *.cma *.so
	/bin/rm -fr html

install: all
	mkdir -p $(APRON_PREFIX)/include
	cp $(H_FILES) $(APRON_PREFIX)/include
	mkdir -p $(APRON_PREFIX)/lib
	for i in libapron.a libapron_debug.a libapron.so libapron_debug.so; do if test -f $$i; then $(INSTALL) $$i $(APRON_PREFIX)/lib; fi; done

distclean:
	/bin/rm -f $(H_FILES:%=$(APRON_PREFIX)/include/%)
	/bin/rm -f $(APRON_PREFIX)/lib/libapron.a
	/bin/rm -f $(APRON_PREFIX)/lib/libapron_debug.a
	/bin/rm -f $(APRON_PREFIX)/lib/libapron.so
	/bin/rm -f $(APRON_PREFIX)/lib/libapron_debug.so
	/bin/rm -f apron.pdf rationale.pdf

#---------------------------------------
# Latex rules
#---------------------------------------

.PHONY : html

ap_pkgrid.texi: ../products/ap_pkgrid.texi
	ln -sf $< $@
ap_ppl.texi: ../ppl/ap_ppl.texi
	ln -sf $< $@
newpolka.texi: ../newpolka/newpolka.texi
	ln -sf $< $@
box.texi: ../box/box.texi
	ln -sf $< $@

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

libapron.a: $(C_FILES:%.c=%.o) \
$(C_FILES_AUX:%.c=%MPQ.o) \
$(C_FILES_AUX:%.c=%D.o) \
$(C_FILES_AUX:%.c=%MPFR.o) \
../itv/libitv.a
	cp ../itv/libitv.a $@
	$(AR) rs $@ $(C_FILES:%.c=%.o) \
		$(C_FILES_AUX:%.c=%MPQ.o) \
		$(C_FILES_AUX:%.c=%D.o) \
		$(C_FILES_AUX:%.c=%MPFR.o)
	$(RANLIB) $@

libapron_debug.a: $(C_FILES:%.c=%_debug.o) \
$(C_FILES_AUX:%.c=%MPQ_debug.o) \
$(C_FILES_AUX:%.c=%D_debug.o) \
$(C_FILES_AUX:%.c=%MPFR_debug.o) \
../itv/libitv_debug.a
	cp ../itv/libitv_debug.a $@
	$(AR) rs $@ $(C_FILES:%.c=%_debug.o) \
		$(C_FILES_AUX:%.c=%MPQ_debug.o) \
		$(C_FILES_AUX:%.c=%D_debug.o) \
		$(C_FILES_AUX:%.c=%MPFR_debug.o)
	$(RANLIB) $@

libapron.so: $(C_FILES:%.c=%.o) \
$(C_FILES_AUX:%.c=%MPQ.o) \
$(C_FILES_AUX:%.c=%D.o) \
$(C_FILES_AUX:%.c=%MPFR.o) \
../itv/libitv.a
	$(CC) $(CFLAGS) -shared -o $@ $(C_FILES:%.c=%.o) \
	$(C_FILES_AUX:%.c=%MPQ.o) \
	$(C_FILES_AUX:%.c=%D.o) \
	$(C_FILES_AUX:%.c=%MPFR.o) \
	$(patsubst %,../itv/%,$(filter %.o, $(shell $(AR) t ../itv/libitv.a))) \
	$(LIBS)

libapron_debug.so: $(C_FILES:%.c=%_debug.o) \
$(C_FILES_AUX:%.c=%MPQ_debug.o) \
$(C_FILES_AUX:%.c=%D_debug.o) \
$(C_FILES_AUX:%.c=%MPFR_debug.o) \
../itv/libitv_debug.a
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $(C_FILES:%.c=%_debug.o) \
	$(C_FILES_AUX:%.c=%MPQ_debug.o) \
	$(C_FILES_AUX:%.c=%D_debug.o) \
	$(C_FILES_AUX:%.c=%MPFR_debug.o) \
	$(patsubst %,../itv/%,$(filter %.o, $(shell $(AR) t ../itv/libitv_debug.a))) \
	$(LIBS)

.PHONY : dep

dep: $(H_FILES)
	for i in $(C_FILES); do		\
		cpp $(ICFLAGS) -I../itv -MM $$i;	\
	done

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS)  -I../itv -c $<

%MPQ.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -I../num -I../itv -I. -c $< -o $@
%D.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -I../num -I../itv -I. -c $< -o $@
%Dl.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -I../num -I../itv -I. -c $< -o $@
%MPFR.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -I../num -I../itv -I. -c $< -o $@

%_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS)  -I../itv -c -o $@ $<

%MPQ_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -I../num -I../itv -I. -c -o $@ $< -o $@
%D_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_DOUBLE -I../num -I../itv -I. -c -o $@ $< -o $@
%Dl_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGDOUBLE -I../num -I../itv -I. -c -o $@ $< -o $@
%MPFR_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPFR -I../num -I../itv -I. -c -o $@ $< -o $@

#---------------------------------------
# dependencies (generated with make dep)
#---------------------------------------

ap_scalar.o: ap_scalar.c ap_scalar.h ap_config.h
ap_interval.o: ap_interval.c ap_interval.h \
  ap_config.h \
  ap_scalar.h $(MPFR_PREFIX)/include/mpfr.h
ap_coeff.o: ap_coeff.c ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h
ap_dimension.o: ap_dimension.c ap_dimension.h
ap_linexpr0.o: ap_linexpr0.c ap_linexpr0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_dimension.h
ap_lincons0.o: ap_lincons0.c ap_lincons0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_dimension.h
ap_generator0.o: ap_generator0.c ap_generator0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_dimension.h
ap_texpr0.o: ap_texpr0.c ap_texpr0.h ap_dimension.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_linearize.h ap_manager.h ap_expr0.h \
  ap_lincons0.h ap_generator0.h ap_tcons0.h ap_abstract0.h
ap_tcons0.o: ap_tcons0.c ap_tcons0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_texpr0.h ap_dimension.h ap_linexpr0.h ap_lincons0.h
ap_manager.o: ap_manager.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h
ap_abstract0.o: ap_abstract0.c ap_abstract0.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_linearize.h
ap_generic.o: ap_generic.c ap_generic.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_linearize.h ap_abstract0.h
ap_var.o: ap_var.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_var.h
ap_environment.o: ap_environment.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h
ap_linexpr1.o: ap_linexpr1.c ap_linexpr1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_linexpr0.h
ap_lincons1.o: ap_lincons1.c ap_lincons1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_lincons0.h \
  ap_linexpr0.h ap_linexpr1.h
ap_generator1.o: ap_generator1.c ap_generator1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_generator0.h \
  ap_linexpr0.h ap_linexpr1.h
ap_texpr1.o: ap_texpr1.c ap_texpr1.h ap_dimension.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_texpr0.h ap_linexpr0.h ap_environment.h ap_var.h \
  ap_linexpr1.h
ap_tcons1.o: ap_tcons1.c ap_tcons1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_tcons0.h ap_texpr0.h ap_dimension.h ap_linexpr0.h \
  ap_lincons0.h ap_environment.h ap_var.h ap_texpr1.h ap_linexpr1.h
ap_abstract1.o: ap_abstract1.c ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_manager.h ap_abstract0.h ap_expr0.h ap_linexpr0.h \
  ap_dimension.h ap_lincons0.h ap_generator0.h ap_texpr0.h ap_tcons0.h \
  ap_linearize.h ap_expr1.h ap_linexpr1.h ap_environment.h ap_var.h \
  ap_lincons1.h ap_generator1.h ap_texpr1.h ap_tcons1.h ap_abstract1.h
ap_linearize.o: ap_linearize.c ap_linearize.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_abstract0.h \
  ap_linearize_aux.h ../itv/itv_fun.h
ap_reducedproduct.o: ap_reducedproduct.c ap_global0.h ap_config.h \
  ap_scalar.h \
  ap_interval.h \
  ap_coeff.h ap_dimension.h ap_linexpr0.h ap_lincons0.h ap_generator0.h \
  ap_texpr0.h ap_tcons0.h ap_manager.h ap_abstract0.h ap_expr0.h \
  ap_linearize.h ap_reducedproduct.h

ap_scalar_debug.o: ap_scalar.c ap_scalar.h ap_config.h
ap_interval_debug.o: ap_interval.c ap_interval.h \
  ap_config.h \
  ap_scalar.h $(MPFR_PREFIX)/include/mpfr.h
ap_coeff_debug.o: ap_coeff.c ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h
ap_dimension_debug.o: ap_dimension.c ap_dimension.h
ap_linexpr0_debug.o: ap_linexpr0.c ap_linexpr0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_dimension.h
ap_lincons0_debug.o: ap_lincons0.c ap_lincons0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_dimension.h
ap_generator0_debug.o: ap_generator0.c ap_generator0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_dimension.h
ap_texpr0_debug.o: ap_texpr0.c ap_texpr0.h ap_dimension.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_linexpr0.h ap_linearize.h ap_manager.h ap_expr0.h \
  ap_lincons0.h ap_generator0.h ap_tcons0.h ap_abstract0.h
ap_tcons0_debug.o: ap_tcons0.c ap_tcons0.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_texpr0.h ap_dimension.h ap_linexpr0.h ap_lincons0.h
ap_manager_debug.o: ap_manager.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h
ap_abstract0_debug.o: ap_abstract0.c ap_abstract0.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_linearize.h
ap_generic_debug.o: ap_generic.c ap_generic.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_linearize.h ap_abstract0.h
ap_var_debug.o: ap_var.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_var.h
ap_environment_debug.o: ap_environment.c ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h
ap_linexpr1_debug.o: ap_linexpr1.c ap_linexpr1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_linexpr0.h
ap_lincons1_debug.o: ap_lincons1.c ap_lincons1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_lincons0.h \
  ap_linexpr0.h ap_linexpr1.h
ap_generator1_debug.o: ap_generator1.c ap_generator1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_environment.h ap_dimension.h ap_var.h ap_generator0.h \
  ap_linexpr0.h ap_linexpr1.h
ap_texpr1_debug.o: ap_texpr1.c ap_texpr1.h ap_dimension.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_texpr0.h ap_linexpr0.h ap_environment.h ap_var.h \
  ap_linexpr1.h
ap_tcons1_debug.o: ap_tcons1.c ap_tcons1.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_tcons0.h ap_texpr0.h ap_dimension.h ap_linexpr0.h \
  ap_lincons0.h ap_environment.h ap_var.h ap_texpr1.h ap_linexpr1.h
ap_abstract1_debug.o: ap_abstract1.c ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_manager.h ap_abstract0.h ap_expr0.h ap_linexpr0.h \
  ap_dimension.h ap_lincons0.h ap_generator0.h ap_texpr0.h ap_tcons0.h \
  ap_linearize.h ap_expr1.h ap_linexpr1.h ap_environment.h ap_var.h \
  ap_lincons1.h ap_generator1.h ap_texpr1.h ap_tcons1.h ap_abstract1.h
ap_linearize_debug.o: ap_linearize.c ap_linearize.h ap_manager.h ap_coeff.h \
  ap_config.h \
  ap_scalar.h \
  ap_interval.h ap_expr0.h ap_linexpr0.h ap_dimension.h ap_lincons0.h \
  ap_generator0.h ap_texpr0.h ap_tcons0.h ap_abstract0.h \
  ap_linearize_aux.h ../itv/itv_fun.h
ap_reducedproduct_debug.o: ap_reducedproduct.c ap_global0.h ap_config.h \
  ap_scalar.h \
  ap_interval.h \
  ap_coeff.h ap_dimension.h ap_linexpr0.h ap_lincons0.h ap_generator0.h \
  ap_texpr0.h ap_tcons0.h ap_manager.h ap_abstract0.h ap_expr0.h \
  ap_linearize.h ap_reducedproduct.h
