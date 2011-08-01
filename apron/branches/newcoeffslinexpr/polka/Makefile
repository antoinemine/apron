include ../Makefile.config

PREFIX = $(APRON_PREFIX)

SRCDIR = $(shell pwd)

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I../num -I../apron -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

# POLKA_NUMTYPES set in ../Makefile.config
TMPL = $(POLKA_NUMTYPES)

H_TMPL_FILES = pkXXX.h pkXXX_internal.h pkeqXXX.h
C_TMPL_FILES = \
pkXXX_vector.c pkXXX_matrix.c pkXXX_cherni.c \
pkXXX_manager.c pkXXX_user.c \
pkXXX_representation.c pkXXX_constructor.c pkXXX_approximate.c \
pkXXX_test.c pkXXX_extract.c \
pkXXX_meetjoin.c pkXXX_assign.c pkXXX_project.c pkXXX_expandfold.c pkXXX_resize.c \
pkXXX_widening.c pkXXX_closure.c \
pkeqXXX.c

H_ORIG_FILES = mf_qsort.h pk_bit.h pk_satmat.h
C_ORIG_FILES = mf_qsort.c pk_bit.c pk_satmat.c pk_option.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(H_TMPL_FILES)))
C_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(C_TMPL_FILES)))

H_GENERATED_FILES_ALL= \
$(foreach T,$(APRON_NUMTYPES), $(subst XXX,$(T),$(H_TMPL_FILES)))
C_GENERATED_FILES_ALL = \
$(foreach T,$(APRON_NUMTYPES), $(subst XXX,$(T),$(C_TMPL_FILES)))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = $(TMPL:%=pk%.h)

#---------------------------------------
# Rules
#---------------------------------------

all: src libpolka.a 
debug: src libpolka.d.a 
prof: src libpolka.p.a 

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

clean:
	/bin/rm -f *.[ao] *.so

distclean: clean
	/bin/rm -f $(H_GENERATED_FILES_ALL) $(C_GENERATED_FILES_ALL)
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL)
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) $^ $(PREFIX)/include/apron

uninstall:

#---------------------------------------
# C rules
#---------------------------------------

libpolka.a: $(C_FILES:%.c=%.o) 
	$(AR) rcs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libpolka.d.a: $(C_FILES:%.c=%.d.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.d.o)
	$(RANLIB) $@

libpolka.p.a: $(C_FILES:%.c=%.p.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.p.o)
	$(RANLIB) $@

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# Files generation
#---------------------------------------

define generate-file
$(subst XXX,$(1),$(2)): $(2)
	@echo "/* GENERATED FROM $$<, DO NOT MODIFY */" >$$@
	@echo "#line 1 \"$(SRCDIR)/$$<\"" >>$$@
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES) $(C_TMPL_FILES), $(eval $(call generate-file,$(T),$(F)))))

#---------------------------------------
# C generic rules
#---------------------------------------

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.d.o): %.d.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.p.o): %.p.o: %.c
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

#---------------------------------------
# dependencies (generated with make dep)
#---------------------------------------

depend: $(H_FILES) $(C_FILES)
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
