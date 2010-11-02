include ../../../../Makefile.config
include ../Makefile.config

ICFLAGS = \
-I../ \
-I$(GMP_PREFIX)/include -I$(MPFR_PREFIX)/include \
-I$(APRON_PREFIX)/include \

LCFLAGS = \
-L../ \
-L$(APRON_PREFIX)/lib \
-L$(GMP_PREFIX)/lib \
-L$(MPFR_PREFIX)/lib \
-L$(PPL_PREFIX)/lib 


ifneq ($(CSDP_PATH),)
CSDP_LIBS = -L/usr/lib/sse2 -L$(CSDP_PATH)/lib -lsdp -llapack -lf77blas -lcblas -latlas -lgfortran -lm
endif

#SDPA = /home/donquijote/dev/sdpa/sdpa.7.3.0

#SDPA_LIBS =  $(SDPA)/lib/libsdpa.a -L$(SDPA)/mumps/build/lib -ldmumps -lmumps_common -lpord -L$(SDPA)/mumps/build/libseq -lmpiseq -L/usr/lib/sse2/atlas -llapack -lgfortran -L/usr/lib/sse2 -lf77blas -lcblas -latlas -lgfortran -lpthread -L/usr/lib/gcc/i486-linux-gnu/4.3.3 -L/usr/lib/gcc/i486-linux-gnu/4.3.3/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/i486-linux-gnu/4.3.3/../../.. -L/usr/lib/i486-linux-gnu -lpthread -lgfortranbegin -lgfortran -lm

LIBDEBUG = -lt1p_debug -loctD -lpolkaRll_debug -lap_ppl_debug -lboxD_debug -lapron_debug -lmpfr -lgmp -lgmpxx -lppl $(CSDP_LIBS) $(SDPA_LIBS) -lm
LIB = -lt1p -loctD -lpolkaRll -lap_ppl -lboxD -lapron -lmpfr -lgmp -lgmpxx -lppl $(CSDP_LIBS) $(SDPA_LIBS) -lm


all: of_box assign_linexp join join_with_cons join_with_cons_box
debug: of_box_debug assign_linexp_debug join_debug join_with_cons_debug join_with_cons_box_debug

subpoly: subpoly.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

subpoly_debug: subpoly_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interpolateurMorceau_quadratique_uncons: interpolateurMorceau_quadratique_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interpolateurMorceau_quadratique_uncons_debug: interpolateurMorceau_quadratique_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interpolateurMorceau_quadratique: interpolateurMorceau_quadratique.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interpolateurMorceau_quadratique_debug: interpolateurMorceau_quadratique_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interpolateurMorceau_uncons: interpolateurMorceau_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interpolateurMorceau_uncons_debug: interpolateurMorceau_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interpolateurMorceau100: interpolateurMorceau100.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interpolateurMorceau100_debug: interpolateurMorceau100_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interpolateurMorceau: interpolateurMorceau.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interpolateurMorceau_debug: interpolateurMorceau_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

toy1: toy1.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

toy1_debug: toy1_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

toy1_uncons: toy1_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

toy1_uncons_debug: toy1_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

essai_apron_9h45_T1p: essai_apron_9h45_T1p.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

essai_apron_9h45_T1p_debug: essai_apron_9h45_T1p_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

essai_apron_9h50_T1p: essai_apron_9h50_T1p.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

essai_apron_9h50_T1p_debug: essai_apron_9h50_T1p_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

essai_apron_9h50: essai_apron_9h50.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

essai_apron_9h50_debug: essai_apron_9h50_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

essai_apron_9h45bis: essai_apron_9h45bis.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

essai_apron_9h45bis_debug: essai_apron_9h45bis_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

essai_apron_9h45: essai_apron_9h45.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

essai_apron_9h45_debug: essai_apron_9h45_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interval_polyhedra3: interval_polyhedra3.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interval_polyhedra3_debug: interval_polyhedra3_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

interval_polyhedra2: interval_polyhedra2.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

interval_polyhedra2_debug: interval_polyhedra2_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

cosine3_uncons: cosine3_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

cosine3_uncons_debug: cosine3_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

valabs_uncons: valabs_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

valabs_uncons_debug: valabs_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

valabs: valabs.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

valabs_debug: valabs_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

cosine3: cosine3.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

cosine3_debug: cosine3_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

cosine2_without_constraints: cosine2_without_constraints.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

cosine2_without_constraints_debug: cosine2_without_constraints_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

cosine2: cosine2.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

cosine2_debug: cosine2_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

cosine: cosine.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

cosine_debug: cosine_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple8_uncons: exemple8_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple8_uncons_debug: exemple8_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple9_uncons: exemple9_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple9_uncons_debug: exemple9_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple10_uncons: exemple10_uncons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple10_uncons_debug: exemple10_uncons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple10: exemple10.o
	$(CC) -pg $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple10_debug: exemple10_debug.o
	$(CC) -pg $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple9: exemple9.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple9_debug: exemple9_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple8: exemple8.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple8_debug: exemple8_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple7: exemple7.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple7_debug: exemple7_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple6: exemple6.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple6_debug: exemple6_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple5: exemple5.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple5_debug: exemple5_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple4: exemple4.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple4_debug: exemple4_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple3: exemple3.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple3_debug: exemple3_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple2: exemple2.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple2_debug: exemple2_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

exemple1: exemple1.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

exemple1_debug: exemple1_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

beta_neg: beta_neg.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

beta_neg_debug: beta_neg_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

join_with_cons_box: join_with_cons_box.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

join_with_cons_box_debug: join_with_cons_box_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

join_with_cons: join_with_cons.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

join_with_cons_debug: join_with_cons_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

join: join.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

join_debug: join_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

assign_linexp: assign_linexp.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

assign_linexp_debug: assign_linexp_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

of_box: of_box.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

of_box_debug: of_box_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

aff_build: aff_build.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIB)

aff_build_debug: aff_build_debug.o
	$(CC) $(SDP_CFLAGS) $(LCFLAGS) -o $@ $< $(LIBDEBUG)

%.o: %.c
	$(CC) $(CFLAGS) $(ICFLAGS) -c -o $@ $<

%_debug.o: %.c
	$(CC) $(CFLAGS_DEBUG) $(ICFLAGS) -c -o $@ $<

clean:
	rm -rf *.[o] *_debug
