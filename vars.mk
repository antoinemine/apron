# Definitions of common variables.

SRCROOT ?= ..

APRON_INCLUDE = $(APRON_PREFIX)/include
APRON_LIB = $(APRON_PREFIX)/lib
APRON_BIN = $(APRON_PREFIX)/bin

ifneq ($(OCAMLFIND),)
  MLGMPIDL_INCLUDE = $(shell $(OCAMLFIND) query gmp)
  MLGMPIDL_LIB = $(MLGMPIDL_INCLUDE)
else
  MLGMPIDL_INCLUDE = $(MLGMPIDL_PREFIX)/include
  MLGMPIDL_LIB = $(MLGMPIDL_PREFIX)/lib
endif

# IC/LI-FLAGS

ICFLAGS = $(CPPFLAGS)

APRON_ICFLAGS = -I$(SRCROOT)/apron
APRON_LIFLAGS = -L$(SRCROOT)/apron
APRON_CMXSINC = -I $(SRCROOT)/apron
NUM_ICFLAGS = -I$(SRCROOT)/num
NUM_LIFLAGS =
ITV_ICFLAGS = -I$(SRCROOT)/itv
ITV_LIFLAGS =

MP_ICFLAGS =
MP_LIFLAGS =
ifneq ($(GMP_PREFIX),)
  MP_ICFLAGS += -I$(GMP_PREFIX)/include
  MP_LIFLAGS += -L$(GMP_PREFIX)/lib
endif
ifneq ($(MPFR_PREFIX),)
  ifneq ($(MPFR_PREFIX),$(GMP_PREFIX))
    MP_ICFLAGS += -I$(MPFR_PREFIX)/include
    MP_LIFLAGS += -L$(MPFR_PREFIX)/lib
  endif
endif

BASE_ICFLAGS = $(APRON_ICFLAGS) $(NUM_ICFLAGS) $(ITV_ICFLAGS) $(MP_ICFLAGS)
BASE_LIFLAGS = $(APRON_LIFLAGS) $(NUM_LIFLAGS) $(ITV_LIFLAGS) $(MP_LIFLAGS)

ML_ICFLAGS =
OCAMLINC =
ifneq ($(HAS_OCAML),)
  ML_ICFLAGS += -I$(MLGMPIDL_INCLUDE) -I$(SRCROOT)/mlapronidl \
		-I$(CAMLIDL_PREFIX) -I$(CAML_PREFIX)
  OCAMLINC   += -I $(MLGMPIDL_LIB) -I $(SRCROOT)/mlapronidl
endif

PPL_ICFLAGS =
PPL_LIFLAGS =
ifneq ($(PPL_PREFIX),)
  PPL_ICFLAGS += -I$(PPL_PREFIX)/include
  PPL_LIFLAGS += -L$(PPL_PREFIX)/lib
endif

PPLITE_ICFLAGS =
PPLITE_LIFLAGS =
ifneq ($(FLINT_PREFIX),)
  PPLITE_ICFLAGS += -I$(FLINT_PREFIX)/include
  PPLITE_LIFLAGS += -L$(FLINT_PREFIX)/lib
endif
ifneq ($(PPLITE_PREFIX),)
  PPLITE_ICFLAGS += -I$(PPLITE_PREFIX)/include
  PPLITE_LIFLAGS += -L$(PPLITE_PREFIX)/lib
endif
 
GLPK_ICFLAGS =
GLPK_LIFLAGS =
ifneq ($(GLPK_PREFIX),)
  GLPK_ICFLAGS += -I$(GLPK_PREFIX)/include
  GLPK_LIFLAGS += -L$(GLPK_PREFIX)lib
endif

# ---

OCAMLOPT_TARGETS0 = $(addsuffix .cmxa, $(1)) $(addsuffix .a, $(1))

ifneq ($(HAS_SHARED),)
  CC_APRON_DYLIB = $(CC) -shared
  CXX_APRON_DYLIB = $(CXX) -shared
  ifneq ($(HAS_OCAML),)
    OCAMLMKLIB := $(OCAMLMKLIB) -dllpath $(APRON_LIB)
    OCAMLMKLIB := $(OCAMLMKLIB) -L$(APRON_LIB)
  endif
  ifneq ($(LINUX_INSTALL_SONAME),)
    CC_APRON_DYLIB += -Wl,-soname,$@
    CXX_APRON_DYLIB += -Wl,-soname,$@
  endif
  ifneq ($(ABSOLUTE_DYLIB_INSTALL_NAMES),)
    CC_APRON_DYLIB += -install_name $(APRON_LIB)/$@
    CXX_APRON_DYLIB += -install_name $(APRON_LIB)/$@
  endif
  ifneq ($(HAS_NATIVE_PLUGINS),)
    OCAMLOPT_CMXS = $(OCAMLOPT) $(OCAMLOPTFLAGS) -linkall -shared $(patsubst %,-cclib %,$(MP_LIFLAGS))
    OCAMLOPT_TARGETS = $(call OCAMLOPT_TARGETS0,$(1)) $(addsuffix .cmxs, $(1))
  else
    OCAMLOPT_TARGETS = $(call OCAMLOPT_TARGETS0,$(1))
  endif
endif

OCAMLPACK := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))ocamlpack
