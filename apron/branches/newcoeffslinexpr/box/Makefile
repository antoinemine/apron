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

TMPL = MPQ D MPFR

H_TMPL_FILES = \
boxXXX.h boxXXX_internal.h
C_TMPL_FILES = \
boxXXX_internal.c boxXXX_representation.c boxXXX_constructor.c boxXXX_meetjoin.c boxXXX_assign.c boxXXX_resize.c boxXXX_otherops.c

H_ORIG_FILES =
C_ORIG_FILES =

H_SRC_FILES = $(H_ORIG_FILES) $(H_TMPL_FILES)
C_SRC_FILES = $(C_ORIG_FILES) $(C_TMPL_FILES)

H_GENERATED_FILES = \
$(TMPL:%=box%.h) $(TMPL:%=box%_internal.h)
C_GENERATED_FILES = \
$(TMPL:%=box%_internal.c) $(TMPL:%=box%_representation.c) $(TMPL:%=box%_constructor.c) $(TMPL:%=box%_meetjoin.c) $(TMPL:%=box%_assign.c) $(TMPL:%=box%_resize.c) $(TMPL:%=box%_otherops.c)

H_FILES = $(H_ORIG_FILES) $(H_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES) $(C_GENERATED_FILES)

CCINC_TO_INSTALL = box.h
CCBIN_TO_INSTALL =
CCLIB_TO_INSTALL = \
$(TMPL:%=libbox%.a) $(TMPL:%=libbox%_debug.a) \
$(TMPL:%=libbox%.so) $(TMPL:%=libbox%_debug.so)

#---------------------------------------
# Rules
#---------------------------------------

all: libbox.a libbox_debug.a
ifneq ($(HAS_SHARED),)
all: libbox.so libbox_debug.so
endif

src: $(H_GENERATED_FILES) $(C_GENERATED_FILES) 

#---------------------------------------
# Misc rules
#---------------------------------------

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

libbox.so: $(C_FILES:%.c=%.o)
	$(CC) $(CFLAGS) -shared -o $@ $(C_FILES:%.c=%.o) 

libbox_debug.so: $(C_FILES:%.c=%_debug.o)
	$(CC) $(CFLAGS_DEBUG) -shared -o $@ $(C_FILES:%.c=%_debug.o)

.PHONY : dep

dep: $(H_FILES) $(C_FILES)
	for i in $(C_FILES); do		\
		cpp $(ICFLAGS) -I../itv -MM $$i;	\
	done

#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# Files generation
#---------------------------------------

define generate-file
@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
macros.pl $(SRCDIR)/$< | $(SED) -e "s/XXX/$*/g; s/$@\([^[:alnum:]]\)/$<\1/g" >>$@
endef

$(TMPL:%=box%.h): box%.h: boxXXX.h
	$(generate-file)
$(TMPL:%=box%_internal.h): box%_internal.h: boxXXX_internal.h
	$(generate-file)
$(TMPL:%=box%_internal.c): box%_internal.c: boxXXX_internal.c
	$(generate-file)
$(TMPL:%=box%_representation.h): box%_representation.h: boxXXX_representation.h
	$(generate-file)
$(TMPL:%=box%_representation.c): box%_representation.c: boxXXX_representation.c
	$(generate-file)
$(TMPL:%=box%_constructor.c): box%_constructor.c: boxXXX_constructor.c
	$(generate-file)
$(TMPL:%=box%_meetjoin.c): box%_meetjoin.c: boxXXX_meetjoin.c
	$(generate-file)
$(TMPL:%=box%_assign.h): box%_assign.h: boxXXX_assign.h
	$(generate-file)
$(TMPL:%=box%_assign.c): box%_assign.c: boxXXX_assign.c
	$(generate-file)
$(TMPL:%=box%_resize.h): box%_resize.h: boxXXX_resize.h
	$(generate-file)
$(TMPL:%=box%_resize.c): box%_resize.c: boxXXX_resize.c
	$(generate-file)
$(TMPL:%=box%_otherops.h): box%_otherops.h: boxXXX_otherops.h
	$(generate-file)
$(TMPL:%=box%_otherops.c): box%_otherops.c: boxXXX_otherops.c
	$(generate-file)

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

depend: $(C_FILES) $(H_FILES)
	$(CC) $(ICFLAGS) -E -MM $(C_FILES) >Makefile.depend

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES)
	etags $^
	etags -a macros.pl
