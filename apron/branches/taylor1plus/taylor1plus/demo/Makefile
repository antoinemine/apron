include ../../../../Makefile.config
include ../Makefile.config

ICFLAGS = \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(APRON_PREFIX)/include \

LCFLAGS = \
-L$(APRON_PREFIX)/lib \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(PPL_PREFIX)/lib 


ifneq ($(CSDP_PATH),)
CSDP_LIBS = -L/usr/lib/sse2 -L$(CSDP_PATH)/lib -lsdp -llapack -lf77blas -lcblas -latlas -lgfortran -lm
endif

#SDPA = /home/donquijote/dev/sdpa/sdpa.7.3.0

#SDPA_LIBS =  $(SDPA)/lib/libsdpa.a -L$(SDPA)/mumps/build/lib -ldmumps -lmumps_common -lpord -L$(SDPA)/mumps/build/libseq -lmpiseq -L/usr/lib/sse2/atlas -llapack -lgfortran -L/usr/lib/sse2 -lf77blas -lcblas -latlas -lgfortran -lpthread -L/usr/lib/gcc/i486-linux-gnu/4.3.3 -L/usr/lib/gcc/i486-linux-gnu/4.3.3/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/i486-linux-gnu/4.3.3/../../.. -L/usr/lib/i486-linux-gnu -lpthread -lgfortranbegin -lgfortran -lm

LIBDEBUG = -lt1p_debug -loctD -lpolka_debug -lap_ppl_debug -lboxD_debug -lapron_debug -lmpfr -lgmp -lgmpxx -lppl $(CSDP_LIBS) $(SDPA_LIBS) -lm
LIB = -lt1p -loctD -lpolka -lap_ppl -lboxD -lapron -lmpfr -lgmp -lgmpxx -lppl $(CSDP_LIBS) $(SDPA_LIBS) -lm

target = filter_o2 filter_o8 sqrt_householder filter_o2_debug filter_o8_debug sqrt_householder_debug

all: filter_o2 filter_o8 sqrt_householder
debug: filter_o2_debug filter_o8_debug sqrt_householder_debug

filter_o2_sylvie: filter_o2_demo1.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

filter_o2_sylvie_debug: filter_o2_demo1_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

filter_o2: filter_o2_demo.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

filter_o2_poster: filter_o2_poster.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

filter_o2_debug: filter_o2_demo_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

filter_o8: filter_o8_astrium.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

filter_o8_debug: filter_o8_astrium_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

inverse: inverse_demo.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

inverse_debug: inverse_demo_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

sqrt_householder: sqrt_householder_demo.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

sqrt_householder_debug: sqrt_householder_demo_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

multi: multi.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

multi_debug: multi_debug.o
	gcc $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

%.o: %.c
	gcc $(CFLAGS) $(ICFLAGS) -c -o $@ $<

%_debug.o: %.c
	gcc $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

clean:
	rm -rf $(target) *.[o]
