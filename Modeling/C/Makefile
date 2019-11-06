IDIR=./includes
ODIR=./outputs
SDIR=./src

CC=gcc
CFLAGS=-I$(IDIR)


_DEPS = axi_ikinematics.h hexapod_gaits.h reg_map.h
DEPS  = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ikinematics_gait_test.o axi_ikinematics.o hexapod_gaits.o
OBJ  = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

ikinematics_gait_test: $(OBJ)
	@echo "Compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)

test_devmem: $(OBJ)
	@echo "compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)*~
