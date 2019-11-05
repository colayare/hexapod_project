IDIR=./includes
ODIR=./outputs

CC=gcc
CFLAGS=-I$(IDIR)


_DEPS = axi_ikinematics.h hexapod_gaits.h reg_map.h
DEPS  = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = axi_ikinematics.o hexapod_gaits.o ikinematics_gait_test.o
OBJ  = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
        $(CC) -c -o $@ $< $(CFLAGS)

ikinematics_gait_test: $(OBJ)
        $(CC) -o $@ $^ $(CFLAGS)

clean:
        rm -f $(ODIR)*.o *~ core $(INCDIR)*~