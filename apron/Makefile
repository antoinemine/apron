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
YYY = expr cons gen

H_TMPL_FILES_XXX = ap_linexprXXX.h ap_linconsXXX.h ap_lingenXXX.h
H_TMPL_FILES_yyy = ap_linyyy0.h ap_linyyy1.h ap_linyyy1_array.h 
H_TMPL_FILES_tmpl = \
ap_linexprconsgen_types.h.tmpl ap_coeff.h.tmpl ap_box0.h.tmpl
H_TMPL_FILES = $(H_TMPL_FILES_XXX) $(H_TMPL_FILES_yyy) $(H_TMPL_FILES_tmpl) 

C_TMPL_FILES_XXX = ap_linexprXXX.c ap_linconsXXX.c ap_lingenXXX.c
C_TMPL_FILES_yyy = ap_linyyy0.c ap_linyyy1.c ap_linyyy1_array.c 
C_TMPL_FILES_tmpl = \
ap_linexprconsgen_conv.c.tmpl ap_coeff.c.tmpl ap_box0.c.tmpl \
ap_check0.c.tmpl ap_generic.c.tmpl
C_TMPL_FILES_yyyXXX = ap_linyyyXXX_array.c

C_TMPL_FILES = \
$(C_TMPL_FILES_XXX) $(C_TMPL_FILES_yyy) $(C_TMPL_FILES_tmpl) \
$(C_TMPL_FILES_yyyXXX)

H_ORIG_FILES = \
ap_config.h ap_dimension.h ap_texpr0.h ap_tcons0.h \
ap_manager.h ap_var.h ap_environment.h \
ap_check0.h ap_abstract0.h \
ap_generic.h

C_ORIG_FILES = \
ap_dimension.c ap_texpr0.c ap_tcons0.c \
ap_manager.c ap_abstract0.c \
ap_var.c ap_environment.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(H_TMPL_FILES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(H_TMPL_FILES_yyy))) \
$(patsubst %.tmpl,%,$(H_TMPL_FILES_tmpl))
C_GENERATED_FILES = \
$(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_XXX))) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(C_TMPL_FILES_yyy))) \
$(patsubst %.tmpl,%,$(C_TMPL_FILES_tmpl)) \
$(foreach Y,$(YYY),$(subst yyy,$(Y),$(foreach T,$(TMPL),$(subst XXX,$(T),$(C_TMPL_FILES_yyyXXX)))))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(H_FILES)
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = 

#---------------------------------------
# Rules
#---------------------------------------

all: libapron.a libapron_debug.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

showsrc:
	@echo $(H_SRC_FILES) $(C_SRC_FILES)
showgen:
	@echo $(H_GENERATED_FILES) $(C_GENERATED_FILES)

#---------------------------------------
# Misc rules
#---------------------------------------

clean:
	/bin/rm -f *.[ao]
	/bin/rm -fr html
distclean: clean
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALLd) $(PREFIX)/include 
	$(INSTALL) $(CCINC_TO_INSTALL) $(PREFIX)/include

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

libapron.a: $(C_FILES:%.c=%.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libapron_debug.a: $(C_FILES:%.c=%_debug.o)
	$(AR) rcs $@ $(C_FILES:%.c=%_debug.o)
	$(RANLIB) $@

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
	./macros.pl $(SRCDIR)/$< >>$@
%.c: %.c.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	./macros.pl $(SRCDIR)/$< >>$@

define generate-file-XXX
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	./macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES_XXX) $(C_TMPL_FILES_XXX),$(eval $(call generate-file-XXX,$(T),$(F)))))

define generate-file-yyy
$(subst yyy,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	./macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g;s/$$@\([^[:alnum:]]\)/$$<\1/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach F,$(H_TMPL_FILES_yyy) $(C_TMPL_FILES_yyy),$(eval $(call generate-file-yyy,$(Y),$(F)))))

define generate-file-yyyXXX
$(subst yyy,$(1),$(subst XXX,$(2),$(3))): $(3)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	./macros.pl $(SRCDIR)/$$< | $(SED) -e "s/yyy/$(1)/g; s/XXX/$(2)/g; s/$$@\([^[:alnum:]]\)/$$<\1/g" >>$$@
endef

$(foreach Y,$(YYY),$(foreach T,$(TMPL),$(foreach F,$(C_TMPL_FILES_yyyXXX),$(eval $(call generate-file-yyyXXX,$(Y),$(T),$(F))))))

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

Makefile.depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend
depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl

