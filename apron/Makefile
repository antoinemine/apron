include ../Makefile.config
include ../vars.mk
include ../version.mk
-include depend

#---------------------------------------
# C part
#---------------------------------------

ICFLAGS += $(NUM_ICFLAGS) $(ITV_ICFLAGS) $(MP_ICFLAGS)
OCAMLINC = -I $(MLGMPIDL)

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
ap_reducedproduct.h \
ap_disjunction.h \
ap_version.h

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
ap_reducedproduct.c \
ap_disjunction.c 

C_FILES_AUX = ap_linearize_aux.c
H_FILES_AUX = ap_linearize_aux.h
CH_FILES_AUX = $(H_FILES_AUX) $(C_FILES_AUX)

LDFLAGS += $(MP_LIFLAGS) -lm -lmpfr -lgmp

#---------------------------------------
# Rules
#---------------------------------------

LIB_FILES = libapron.a
LIB_FILES_DEBUG = libapron_debug.a
ifneq ($(HAS_SHARED),)
LIB_FILES += libapron.$(EXT_DLL)
LIB_FILES_DEBUG += libapron_debug.$(EXT_DLL)
endif

all: $(LIB_FILES) $(LIB_FILES_DEBUG)

#---------------------------------------
# Misc rules
#---------------------------------------

tar: $(H_FILES) $(C_FILES) $(CH_FILES_AUX) apron.texi Makefile COPYING README
	(cd ..; tar zcvf apron.tgz $(^:%=apron/%))

dist: $(H_FILES) $(C_FILES) $(CH_FILES_AUX) apron.texi Makefile COPYING README apron.info apron.pdf html
	(cd ..; tar zcvf apron.tgz $(^:%=apron/%))

clean:
	/bin/rm -f *.aux *.bbl *.blg *.dvi *.log *.toc *.ps *.pdf apron.cps apron.fns apron.info apron.fn apron.ky apron.pg apron.cp apron.tp apron.vr apron.kys apron.pgs apron.tps apron.vrs newpolka.texi box.texi ap_ppl.texi ap_pplite.texi ap_pkgrid.texi apron.info* depend ap_version.h
	/bin/rm -f *.o *.a *.cmi *.cmo *.cmx *.cmx[as] *.cma *.$(EXT_DLL)
	/bin/rm -fr html
	/bin/rm -f apron.pdf rationale.pdf

distclean: clean

install: all
	mkdir -p $(APRON_INCLUDE)
	cp $(H_FILES) $(APRON_INCLUDE)
	mkdir -p $(APRON_LIB)
	for i in $(LIB_FILES); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); $(STRIP) $(APRON_LIB)/$$i;  fi; done
ifneq ($(HAS_DEBUG),)
	for i in $(LIB_FILES_DEBUG); do if test -f $$i; then $(INSTALL) $$i $(APRON_LIB); fi; done
endif

uninstall:
	/bin/rm -f $(H_FILES:%=$(APRON_INCLUDE)/%)
	/bin/rm -f $(APRON_LIB)/libapron.a
	/bin/rm -f $(APRON_LIB)/libapron_debug.a
	/bin/rm -f $(APRON_LIB)/libapron.$(EXT_DLL)
	/bin/rm -f $(APRON_LIB)/libapron_debug.$(EXT_DLL)

#---------------------------------------
# Latex rules
#---------------------------------------

.PHONY : html

ap_pkgrid.texi: ../products/ap_pkgrid.texi
	ln -sf $< $@
ap_ppl.texi: ../ppl/ap_ppl.texi
	ln -sf $< $@
ap_pplite.texi: ../pplite/ap_pplite.texi
	ln -sf $< $@
newpolka.texi: ../newpolka/newpolka.texi
	ln -sf $< $@
box.texi: ../box/box.texi
	ln -sf $< $@

apron.pdf: apron.texi rationale.texi ../products/ap_pkgrid.texi ../ppl/ap_ppl.texi ../pplite/ap_pplite.texi ../newpolka/newpolka.texi ../box/box.texi 
	$(TEXI2ANY) --pdf -o $@ $<

apron.info: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi ap_pplite.texi newpolka.texi box.texi
	$(MAKEINFO) -o $@ $<


html: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi ap_pplite.texi newpolka.texi box.texi 
	$(TEXI2ANY) -o html --html --split=section --no-number-sections $<
	cp -f ../octagons/oct_doc.html html
	cp -f ../examples/*.c ../examples/*.ml html

#---------------------------------------
# C rules
#---------------------------------------

O_FILES = \
$(C_FILES:%.c=%.o) \
$(C_FILES_AUX:%.c=%MPQ.o) \
$(C_FILES_AUX:%.c=%D.o) \
$(C_FILES_AUX:%.c=%MPFR.o)

O_FILES_DEBUG = \
$(C_FILES:%.c=%_debug.o) \
$(C_FILES_AUX:%.c=%MPQ_debug.o) \
$(C_FILES_AUX:%.c=%D_debug.o) \
$(C_FILES_AUX:%.c=%MPFR_debug.o)

libapron.a: $(O_FILES) ../itv/libitv.a
	cp ../itv/libitv.a $@
	$(AR) rs $@ $(O_FILES)
	$(RANLIB) $@

libapron_debug.a: $(O_FILES_DEBUG) ../itv/libitv_debug.a
	cp ../itv/libitv_debug.a $@
	$(AR) rs $@ $(O_FILES_DEBUG)
	$(RANLIB) $@

libapron.$(EXT_DLL): $(O_FILES) ../itv/libitv.a
	$(CC_APRON_DYLIB) $(CFLAGS) -o $@ $(O_FILES) $(patsubst	\
		%,../itv/%,$(filter %.o, $(shell $(AR) t	\
		../itv/libitv.a))) $(LDFLAGS)

libapron_debug.$(EXT_DLL): $(O_FILES_DEBUG) ../itv/libitv_debug.a
	$(CC_APRON_DYLIB) $(CFLAGS_DEBUG) -o $@ $(O_FILES_DEBUG)	\
		$(patsubst %,../itv/%,$(filter %.o, $(shell $(AR) t	\
		../itv/libitv_debug.a))) $(LDFLAGS)

ap_version.h: ../version.mk
	echo "// APRON version, automatically generated from version.mk" > $@
	echo "#ifndef AP_VERSION" >> $@
	echo "#define AP_VERSION_MAJOR ${VERSION_MAJOR}" >> $@
	echo "#define AP_VERSION_MINOR ${VERSION_MINOR}" >> $@
	echo "#define AP_VERSION_MICRO ${VERSION_MICRO}" >> $@
	echo "#define AP_VERSION       \"${VERSION_STR}\"" >> $@
	echo "#endif" >> $@

.PHONY : dep depend

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# C generic rules
#---------------------------------------

%.o: %.c %.h ap_version.h
	$(CC) $(CFLAGS) $(ICFLAGS) -c $<

%MPQ.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPQ -I. -c $< -o $@
%D.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_DOUBLE -I. -c $< -o $@
%Dl.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_LONGDOUBLE -I. -c $< -o $@
%MPFR.o: %.c %.h
	$(CC) $(CFLAGS) $(ICFLAGS) -DNUM_MPFR -I. -c $< -o $@

%_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

%MPQ_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPQ -I. -c -o $@ $< -o $@
%D_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_DOUBLE -I. -c -o $@ $< -o $@
%Dl_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_LONGDOUBLE -I. -c -o $@ $< -o $@
%MPFR_debug.o: %.c %.h
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -DNUM_MPFR -I. -c -o $@ $< -o $@

#---------------------------------------
# dependencies (generated with make depend)
#---------------------------------------

depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) > depend

.precious: ap_version.h
