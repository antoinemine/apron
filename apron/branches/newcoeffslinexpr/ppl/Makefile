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

CXX_TMPL_FILES_tmpl = ppl_user.cc.tmpl ppl_poly.cc.tmpl ppl_grid.cc.tmpl
HXX_TMPL_FILES_tmpl = 
CXX_ORIG_FILES = 
#CSOURCES = ppl_test.c
HXX_ORIG_FILES = ppl_user.hh ppl_poly.hh ppl_grid.hh 
C_ORIG_FILES = 
H_ORIG_FILES = ppl_grid.h ap_ppl.h

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

.PRECIOUS: $(HXX_GENERATED_FILES) $(CXX_GENERATED_FILES)

#---------------------------------------
# Rules
#---------------------------------------

all: src libap_ppl.a 
debug: src libap_ppl.d.a 
prof: src libap_ppl.p.a 

src: 

clean:
	/bin/rm -f *.[ao] *.so

distclean: clean
	/bin/rm -f Makefile.depend

install: $(CCINC_TO_INSTALL)
	$(INSTALL) -d $(PREFIX)/include/apron
	$(INSTALL) $^ $(PREFIX)/include/apron

uninstall:

#---------------------------------------
# C rules
#---------------------------------------

libap_ppl.a: $(C_FILES:%.c=%.o) $(CXX_FILES:%.cc=%.o) 
	$(AR) rcs $@ $^
	$(RANLIB) $@

libap_ppl.d.a: $(C_FILES:%.c=%.d.o) $(CXX_FILES:%.cc=%.d.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

libap_ppl.p.a: $(C_FILES:%.c=%.p.o) $(CXX_FILES:%.cc=%.p.o)
	$(AR) rcs $@ $^
	$(RANLIB) $@

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
