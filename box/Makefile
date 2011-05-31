include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Directories
#---------------------------------------

#---------------------------------------
# Programs
#---------------------------------------

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I$(MLGMPIDL_PREFIX)/include \
-I../num \
-I../apron \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \

#---------------------------------------
# Files
#---------------------------------------

# BOX_NUMTYPES set in ../Makefile.config
TMPL = $(BOX_NUMTYPES)

H_TMPL_FILES = \
boxXXX.h boxXXX_internal.h
C_TMPL_FILES = \
boxXXX_internal.c boxXXX_representation.c boxXXX_constructor.c boxXXX_meetjoin.c boxXXX_assign.c boxXXX_resize.c boxXXX_otherops.c

H_ORIG_FILES = 
C_ORIG_FILES = 

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(H_TMPL_FILES)))
C_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(C_TMPL_FILES)))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = box.h $(TMPL:%=box%.h)

#---------------------------------------
# Rules
#---------------------------------------

all: src libbox.a
debug: src libbox.d.a
prof: src libbox.p.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

clean:
	/bin/rm -f *.[ao] *.so

mostlyclean distclean: clean
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL) 
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) -t $(PREFIX)/include/apron $^

uninstall:

#---------------------------------------
# C rules
#---------------------------------------

libbox.a: $(C_FILES:%.c=%.o) 
	$(AR) rcs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libbox.d.a: $(C_FILES:%.c=%.d.o)
	$(AR) rcs $@ $(C_FILES:%.c=%.d.o)
	$(RANLIB) $@

libbox.p.a: $(C_FILES:%.c=%.p.o)
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
	../macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/#line \([0-9]*\) \"\(.*\)$$@\"/#line \1 \"\2$$<\"/g" >>$$@
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
