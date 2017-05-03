EXENAME = main
CC = mpicc
CFLAGS = -I. -std=c99 -Werror -Wall -Wextra -pedantic

DEPS = args.h data.h ols_alg.h
OBJ = args.o data.o ols_alg.o main.o

MKLROOT = /opt/intel/compilers_and_libraries_2017.2.174/linux/mkl
INTELMKLFLAGS = -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_scalapack_ilp64 -lmkl_intel_ilp64 -lmkl_gnu_thread -lmkl_core -lmkl_blacs_intelmpi_ilp64 -lgomp -lpthread -lm -ldl
COPTIONS = -DMKL_ILP64 -m64 -I$(MKLROOT)/include

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(COPTIONS) $(INTELMKLFLAGS)

$(EXENAME) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(COPTIONS) $(INTELMKLFLAGS)

.PHONY : clean

clean :
	rm -r -f *.o *~ core $(EXENAME)

