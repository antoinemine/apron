include ../Makefile.config

#---------------------------------------
# Directories
#---------------------------------------

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
-I$(MLGMPIDL_PREFIX)/include \
-I../num \
-I../apron \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \

#---------------------------------------
# Files
#---------------------------------------

TMPL = Ill D MPQ MPFR

H_TMPL_FILES = \
boxXXX.h boxXXX_internal.h
C_TMPL_FILES = \
boxXXX_internal.c boxXXX_representation.c boxXXX_constructor.c boxXXX_meetjoin.c boxXXX_assign.c boxXXX_resize.c boxXXX_otherops.c

H_ORIG_FILES = box.h
C_ORIG_FILES = box.c

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(H_TMPL_FILES)))
C_GENERATED_FILES = \
$(foreach T,$(TMPL), $(subst XXX,$(T),$(C_TMPL_FILES)))

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = box.h $(TMPL:%=box%.h)
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL =

#---------------------------------------
# Rules
#---------------------------------------

all: libbox.a libbox_debug.a

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

clean:
	/bin/rm -f *.o *.a *.so
	/bin/rm -f $(H_GENERATED_FILES) $(C_GENERATED_FILES)

distclean: clean
	/bin/rm -f Makefile.depend

install: all
	$(INSTALLd) $(APRON_PREFIX)/include
	$(INSTALLd) $(APRON_PREFIX)/lib
	for i in $(CCINC_TO_INSTALL); if test -f $$i; then  $(INSTALL) $$i $(APRON_PREFIX)/include; fi done
	for i in $(CCLIB_TO_INSTALL); do if test -f $$i; then $(INSTALL) $$i $(APRON_PREFIX)/lib; fi; done

uninstall:
	/bin/rm -f $(CCINC_TO_INSTALL:%=$(APRON_PREFIX)/include/%)
	/bin/rm -f $(CCLIB_TO_INSTALL:%=$(APRON_PREFIX)/lib/%)

#---------------------------------------
# C rules
#---------------------------------------

libbox.a: $(C_FILES:%.c=%.o) 
	$(AR) rcs $@ $(C_FILES:%.c=%.o) 
	$(RANLIB) $@

libbox_debug.a: $(C_FILES:%.c=%_debug.o)
	$(AR) rcs $@ $(C_FILES:%.c=%_debug.o)
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
	./macros.pl $(SRCDIR)/$$< | $(SED) -e "s/XXX/$(1)/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$$@
endef

$(foreach T,$(TMPL),$(foreach F,$(H_TMPL_FILES) $(C_TMPL_FILES), $(eval $(call generate-file,$(T),$(F)))))

#---------------------------------------
# C generic rules
#---------------------------------------

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%_debug.o): %_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<


#---------------------------------------
# dependencies (generated with make dep)
#---------------------------------------

Makefile.depend depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl
