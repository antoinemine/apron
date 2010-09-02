LOCAL=$(HOME)/dev
#---------------------
PROF=$(LOCAL)/ProfilBias
SDPA =$(LOCAL)/sdpa/sdpa.7.3.0

# packet debian standard libarpack++2-dev
#ARPACK_P =$(LOCAL)/arpack++
#ARPACK = $(LOCAL)/ARPACK

# packet debian standard libsuitesparse-dev
#UMF = $(LOCAL)/umfpack/umfpack
#AMD = $(LOCAL)/umfpack/amd

#CFLAGS = -Wno-deprecated -c -O0 -g
CFLAGS = -Wno-deprecated -c -O3 -fomit-frame-pointer

LFLAGS = -z muldefs

sdpl: sdpl.o vSDP.o vSDP_struct.o arpack_interface.o infeassdp.o
	g++ -o sdpl $(LFLAGS) sdpl.o vSDP.o vSDP_struct.o arpack_interface.o infeassdp.o \
	-L$(SDPA) -L$(HOME)/lapack/lib -L$(PROF)/lib \
	-lsdpa -llapack -lcblaswr -lcblas -lf77blas -lI77 -lF77 \
	-lProfilPackages -lProfil -lBias -llr -lm -larpack_linux -lsuperlu_3.0 \
	-lblas -lumfpack -lamd /usr/lib64/libg2c.a

sdpl.o: sdpl.cpp vSDP.h
	g++ $(CFLAGS) sdpl.cpp -I$(SDPA) -I$(HOME)/lapack/include -I$(PROF)/include -I$(PROF)/src/Base

vSDP.o: vSDP.cpp vSDP.h vSDP_struct.h arpack_interface.h init.h
	g++ $(CFLAGS) vSDP.cpp -I$(PROF)/include -I$(PROF)/src/Base -I$(HOME)/lapack/include -I$(SDPA)	

vSDP_struct.o: vSDP_struct.cpp vSDP_struct.h
	g++ $(CFLAGS) vSDP_struct.cpp -I$(PROF)/include -I$(PROF)/src/Base -I$(HOME)/lapack/include -I$(SDPA)

arpack_interface.o: arpack_interface.cpp arpack_interface.h
	g++ $(CFLAGS) arpack_interface.cpp

infeassdp.o: infeassdp.cpp infeassdp.h
	g++ $(CFLAGS) infeassdp.cpp -I$(PROF)/include -I$(SDPA)/include 

clean:
	rm -f *.o sdpl
