# Makefile
#
# APRON Library / PPL library wrapper
#
# Copyright (C) Antoine Mine' 2006

# This file is part of the APRON Library, released under GPL license.
# Please read the COPYING file packaged in the distribution.

include ../Makefile.config

PREFIX = $(APRON_PREFIX)
SRCDIR = $(shell pwd)

#---------------------------------------
# Flags
#---------------------------------------

# Use ICFLAGS to specify machine-independent compilation flags.
ICFLAGS = \
-I$(PPL_PREFIX)/include \
-I../apron -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

ICXXFLAGS = \
-I$(PPL_PREFIX)/include \
-I../apron -I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include

#---------------------------------------
# Files
#---------------------------------------

CXX_TMPL_FILES_tmpl = ppl_user.cc.tmpl ppl_poly.cc.tmpl ppl_grid.cc.tmpl ppl_bd_shape.cc.tmpl
HXX_TMPL_FILES_tmpl = 
CXX_ORIG_FILES = 
#CSOURCES = ppl_test.c
HXX_ORIG_FILES = ppl_user.hh
C_ORIG_FILES = 
H_ORIG_FILES =  ppl_poly.h ppl_grid.h ppl_bd_shape.h apron_ppl.h

CXX_SRC_FILES = $(CXX_ORIG_FILES) $(CXX_TMPL_FILES_tmpl)
HXX_SRC_FILES = $(HXX_ORIG_FILES) $(HXX_TMPL_FILES_tmpl)
C_SRC_FILES = $(C_ORIG_FILES)
H_SRC_FILES = $(H_ORIG_FILES)

CXX_GENERATED_FILES = $(patsubst %.tmpl,%,$(CXX_TMPL_FILES_tmpl))
HXX_GENERATED_FILES = $(patsubst %.tmpl,%,$(HXX_TMPL_FILES_tmpl))

CXX_FILES = $(CXX_ORIG_FILES) $(CXX_GENERATED_FILES)
HXX_FILES = $(HXX_ORIG_FILES) $(HXX_GENERATED_FILES)
C_FILES = $(C_ORIG_FILES)
H_FILES = $(H_ORIG_FILES)

CCINC_TO_INSTALL = $(HXX_FILES) $(H_FILES)

CCLIB_TO_INSTALL_s = libapron_ppl.a
ifneq ($(APRON_DEBUG),)
CCLIB_TO_INSTALL_s += libapron_ppl.d.a
endif
ifneq ($(APRON_PROF),)
CCLIB_TO_INSTALL_s += libapron_ppl.p.a
endif
CCLIB_TO_INSTALL = $(CCLIB_TO_INSTALL_s)
ifneq ($(HAS_SHARED),)
CCLIB_TO_INSTALL += $(CCLIB_TO_INSTALL_s:%.a=%.so)
endif

.PRECIOUS: $(HXX_GENERATED_FILES) $(CXX_GENERATED_FILES)

#---------------------------------------
# Rules
#---------------------------------------

all: src libapron_ppl.a 
debug: src libapron_ppl.d.a 
prof: src libapron_ppl.p.a 
ifneq ($(HAS_SHARED),)
all: src libapron_ppl.so
debug: src libapron_ppl.d.so
prof: src libapron_ppl.p.so
endif

src: $(CXX_GENERATED_FILES) $(HXX_GENERATED_FILES)

clean:
	/bin/rm -f *.[ao] *.so

mostlyclean distclean: clean
	/bin/rm -f Makefile.depend TAGS $(CXX_GENERATED_FILES) $(HXX_GENERATED_FILES)

install: $(CCINC_TO_INSTALL) $(CCLIB_TO_INSTALL)
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) $(CCINC_TO_INSTALL) $(PREFIX)/include/apron
	$(INSTALL) -d $(PREFIX)/lib/apron
	$(INSTALL) $(CCLIB_TO_INSTALL) $(PREFIX)/lib/apron

uninstall:

#---------------------------------------
# C rules
#---------------------------------------

define generate-clib
libapron_ppl$(1).a: $(C_FILES:%.c=%$(1).o) $(CXX_FILES:%.cc=%$(1).o) 
	$(AR) rcs $$@ $$^
	$(RANLIB) $$@
ifneq ($(HAS_SHARED),)
libapron_ppl$(1).so: $(C_FILES:%.c=%$(1).o) $(CXX_FILES:%.cc=%$(1).o) 
	$(CXX) $(CFLAGS) -shared -o $$@ \
	-L$(PPL_PREFIX)/lib -lppl $$^ 
	#-L$(MPFR_PREFIX)/lib -lmpfr -L$(GMP_PREFIX)/lib -lgmp $$^
endif
endef

$(eval $(call generate-clib,))
$(eval $(call generate-clib,.d))
$(eval $(call generate-clib,.p))

#libapron_ppl.a: $(C_FILES:%.c=%.o) $(CXX_FILES:%.cc=%.o) 
#	$(AR) rcs $@ $^
#	$(RANLIB) $@
#libapron_ppl.d.a: $(C_FILES:%.c=%.d.o) $(CXX_FILES:%.cc=%.d.o)
#	$(AR) rcs $@ $^
#	$(RANLIB) $@
#libapron_ppl.p.a: $(C_FILES:%.c=%.p.o) $(CXX_FILES:%.cc=%.p.o)
#	$(AR) rcs $@ $^
#	$(RANLIB) $@


#--------------------------------------------------------------
# IMPLICIT RULES AND DEPENDENCIES
#--------------------------------------------------------------

.SUFFIXES: .c .h .o

#---------------------------------------
# Files generation
#---------------------------------------

%.hh: %.hh.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@
%.cc: %.cc.tmpl
	@echo "/* GENERATED FROM $<, DO NOT MODIFY */" >$@
	@echo "#line 1 \"$(SRCDIR)/$<\"" >>$@
	../macros.pl $(SRCDIR)/$< >>$@

#---------------------------------------
# C/CXX generic rules
#---------------------------------------

$(C_FILES:%.c=%.o): %.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.d.o): %.d.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
$(C_FILES:%.c=%.p.o): %.p.o: %.c
	$(CC) $(CFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

$(CXX_FILES:%.cc=%.o): %.o: %.cc
	$(CXX) $(CXXFLAGS) $(ICFLAGS) -c -o $@ $<
$(CXX_FILES:%.cc=%.d.o): %.d.o: %.cc
	$(CXX) $(CXXFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<
$(CXX_FILES:%.cc=%.p.o): %.p.o: %.cc
	$(CXX) $(CXXFLAGS_PROF) $(ICFLAGS) -c -o $@ $<

#---------------------------------------
# dependencies (generated with make dep)
#---------------------------------------

depend: $(H_FILES) $(C_FILES) $(HXX_FILES) $(CXX_FILES)
	$(CXX) $(ICFLAGS) -E -MM $(C_FILES) $(CXX_FILES) >Makefile.depend.pattern
	cp Makefile.depend.pattern Makefile.depend
	$(SED) -e "s/.o:/.d.o:/g" Makefile.depend.pattern >>Makefile.depend
	$(SED) -e "s/.o:/.p.o:/g" Makefile.depend.pattern >>Makefile.depend
	rm Makefile.depend.pattern

-include Makefile.depend

.PHONY: tags TAGS
tags: TAGS
TAGS: $(H_SRC_FILES) $(C_SRC_FILES) $(HXX_SRC_FILES) $(CXX_SRC_FILES)
	etags $^
