include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

ICFLAGS = -I. -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

TMPL = $(APRON_NUMTYPES)
YYY = expr cons gen
ZZZ = expr cons
SSS = l ll

H_TMPL_FILES_sss = numIsss.h numRsss.h
H_TMPL_FILES_other = numDsss.h
H_TMPL_FILES_XXX = boundXXX.h itvXXX.h eitvXXX.h ap_linexprXXX_eval.h ap_linconsXXX_eval.h
H_TMPL_FILES_yyy = ap_linyyy0.h ap_linyyy1.h
H_TMPL_FILES_zzz = ap_tzzz1.h
H_TMPL_FILES_tmpl = num_types.h.tmpl numMPQ.h.tmpl num_conv.h.tmpl ap_lin_types.h.tmpl ap_coeff.h.tmpl ap_check0.h.tmpl 
H_TMPL_FILES_yyyXXX = ap_linyyyXXX.h
H_TMPL_FILES = $(H_TMPL_FILES_sss) $(H_TMPL_FILES_other) $(H_TMPL_FILES_XXX) $(H_TMPL_FILES_yyy) $(H_TMPL_FILES_zzz) $(H_TMPL_FILES_tmpl) $(H_TMPL_FILES_yyyXXX)

C_TMPL_FILES_sss = numIsss.c numRsss.c
C_TMPL_FILES_other = numDsss.c
C_TMPL_FILES_XXX = boundXXX.c itvXXX.c eitvXXX.c ap_linexprXXX_eval.c ap_linconsXXX_eval.c
C_TMPL_FILES_yyy = ap_linyyy0.c ap_linyyy1.c
C_TMPL_FILES_zzz = ap_tzzz0_array.c ap_tzzz1.c
C_TMPL_FILES_tmpl = \
num_internal.c.tmpl num_conv.c.tmpl \
ap_lin_conv.c.tmpl ap_coeff.c.tmpl \
ap_check0.c.tmpl ap_abstract0.c.tmpl ap_generic.c.tmpl ap_abstract1.c.tmpl \
ap_reducedproduct.c.tmpl
C_TMPL_FILES_yyyXXX = ap_linyyyXXX.c

C_TMPL_FILES = \
$(C_TMPL_FILES_sss) $(C_TMPL_FILES_other) $(C_TMPL_FILES_XXX) $(C_TMPL_FILES_yyy) $(C_TMPL_FILES_zzz) $(C_TMPL_FILES_tmpl) \
$(C_TMPL_FILES_yyyXXX)

H_ORIG_FILES = \
num_internal.h \
numMPZ.h numMPFR.h \
num_all.h bound_all.h itv_all.h eitv_all.h \
ap_config.h ap_dimension.h ap_texpr0.h ap_tcons0.h \
ap_manager.h ap_var.h ap_environment.h \
ap_check0.h ap_abstract0.h \
ap_generic.h ap_abstract1.h ap_reducedproduct.h \
ap_global0.h ap_global1.h

C_ORIG_FILES = \
numMPZ.c numMPQ.c numMPFR.c \
ap_dimension.c ap_texpr0.c ap_tcons0.c \
ap_manager.c ap_var.c ap_environment.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(foreach S,$(SSS),$(subst sss,$(S),$(H_TMPL_FILES_sss))) \
numD.h numDl.h \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(H_TMPL_FILES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(H_TMPL_FILES_yyy))) \
$(foreach Z,$(ZZZ),$(subst zzz,$(Z),$(H_TMPL_FILES_zzz))) \
$(patsubst %.tmpl,%,$(H_TMPL_FILES_tmpl)) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(foreach T,$(TMPL),$(subst XXX,$(T),$(H_TMPL_FILES_yyyXXX)))))
C_GENERATED_FILES = \
$(foreach S,$(SSS),$(subst sss,$(S),$(C_TMPL_FILES_sss))) \
numD.c numDl.c \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(C_TMPL_FILES_yyy))) \
$(foreach Z,$(ZZZ),$(subst zzz,$(Z),$(C_TMPL_FILES_zzz))) \
$(patsubst %.tmpl,%,$(C_TMPL_FILES_tmpl)) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_yyyXXX)))))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(H_FILES)

.PRECIOUS: $(H_GENERATED_FILES) $(C_GENERATED_FILES)

#---------------------------------------
# Rules
#---------------------------------------

all: src libapron.a 
debug: src libapron.d.a
prof: src libapron.p.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

showsrc:
	@echo $(H_SRC_FILES) $(C_SRC_FILES)
showgen:
	@echo $(H_GENERATED_FILES) $(C_GENERATED_FILES)

clean:
	/bin/rm -f *.[ao]
	/bin/rm -fr html
distclean: clean
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL) 
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) $^ $(PREFIX)/include/apron 

uninstall:

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# Files generation
#---------------------------------------

%.h: %.h.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@

define generate-file-XXX
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES_XXX) $(C_TMPL_FILES_XXX),$(eval $(call generate-file-XXX,$(T),$(F)))))

define generate-file-sss
$(subst sss,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/sss/$(1)/g;s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef
$(foreach S,$(SSS),$(foreach F,numIsss.h numRsss.h numIsss.c numRsss.c,$(eval $(call generate-file-sss,$(S),$(F)))))
define generate-file-sss
$(subst sss,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/www/$(1)/g;s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef
$(foreach F,numDsss.h numDsss.c,$(eval $(call generate-file-sss,,$(F))))
$(foreach F,numDsss.h numDsss.c,$(eval $(call generate-file-sss,l,$(F))))

define generate-file-yyy
$(subst yyy,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g;s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach F,$(H_TMPL_FILES_yyy) $(C_TMPL_FILES_yyy),$(eval $(call generate-file-yyy,$(Y),$(F)))))

define generate-file-zzz
$(subst zzz,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/zzz/$(1)/g;s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Z,$(ZZZ),$(foreach F,$(H_TMPL_FILES_zzz) $(C_TMPL_FILES_zzz),$(eval $(call generate-file-zzz,$(Z),$(F)))))

define generate-file-yyyXXX
$(subst yyy,$(1),$(subst XXX,$(2),$(3))): $(3)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g; s/XXX/$(2)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES_yyyXXX) $(C_TMPL_FILES_yyyXXX),$(eval $(call generate-file-yyyXXX,$(Y),$(T),$(F))))))

#---------------------------------------
# C rules
#---------------------------------------

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.d.o): %.d.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.p.o): %.p.o: %.c
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

libapron.a: $(C_FILES:%.c=%.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libapron.d.a: $(C_FILES:%.c=%.d.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.d.o)
	$(RANLIB) $@

libapron.p.a: $(C_FILES:%.c=%.p.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.p.o)
	$(RANLIB) $@

#---------------------------------------
# dependencies (generated with make depend)
#---------------------------------------

src: $(C_FILES) $(H_FILES)

depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend.pattern
	cp Makefile.depend.pattern Makefile.depend
	$(SED) -e "s/.o:/.d.o:/g" Makefile.depend.pattern >>Makefile.depend
	$(SED) -e "s/.o:/.p.o:/g" Makefile.depend.pattern >>Makefile.depend
	rm Makefile.depend.pattern

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^

#---------------------------------------
# Latex rules
#---------------------------------------

.PHONY : html

ap_pkgrid.texi: ../products/ap_pkgrid.texi
	ln -s $< $@
ap_ppl.texi: ../ppl/ap_ppl.texi
	ln -s $< $@
polka.texi: ../polka/polka.texi
	ln -s $< $@
box.texi: ../box/box.texi
	ln -s $< $@

apron.pdf: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi polka.texi box.texi 
	$(TEXI2DVI) --pdf -o $@ $<

apron.info: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi polka.texi box.texi
	$(MAKEINFO) -o $@ $<


html: apron.texi rationale.texi ap_pkgrid.texi ap_ppl.texi polka.texi box.texi 
	$(TEXI2HTML) -split=section -nonumber -menu -subdir=html $<
	cp -f ../octagons/oct_doc.html html

