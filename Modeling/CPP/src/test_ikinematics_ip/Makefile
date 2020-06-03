# Includes Directory
IDIR=$(MODEL_CPP_PATH)/includes
SDIR=$(MODEL_CPP_PATH)/src/test_ikinematics_ip
# Execution directory set as Outputs Directory
ODIR=$(MODEL_CPP_PATH)/bin
$(info ===============================)
$(info Append Includes dir $(IDIR))
$(info Append Sources dir $(SDIR))
$(info Append Params dir $(PDIR))
$(info Set $(PWD)/outputs as Output Directory)
$(info ===============================)

# 
CC=g++
CFLAGS=-I$(IDIR)
EXT=cpp


_GLOBALS   = axi_ip_globals.h global_defines.h
GLOBALS    = $(patsubst %,$(IDIR)/globals/%,$(_DEPS))

_GLOBALS_OBJ = axi_ip_globals.o linux_timing.o
GLOBALS_OBJ  = $(patsubst %,$(ODIR)/%,$(_GLOBALS_OBJ))

_PARAMS = hexapod_ip_params.h hexapod_params.h ip_reg_map.h
PARAMS  = $(patsubst %,$(IDIR)/params/%,$(_PARAMS))

_HEXA_DEPS 	= hexapod_kinematics.h
HEXA_DEPS	= $(patsubst %,$(IDIR)/hexapod/%,$(_HEXA_DEPS))

_HEXA_OBJ = hexapod_kinematics.o hexapod_locomotion.o
HEXA_OBJ  = $(patsubst %,$(ODIR)/%,$(_HEXA_OBJ))

_TEST_IKINEMATICS_OBJ 	= test_ikinematics.o
TEST_IKINEMATICS_OBJ    = $(patsubst %,$(ODIR)/%,$(_TEST_IKINEMATICS_OBJ))

$(info > Globals)
$(info $(GLOBALS))

$(info > Hexapod Class Dependencies)
$(info $(HEXA_DEPS))

$(info > Parameters)
$(info $(PARAMS))

$(info > Hexapod Objects)
$(info $(HEXA_OBJ))

$(info > Test Objects)
$(info $(TEST_IKINEMATICS_OBJ))

$(ODIR)/%.o: $(SDIR)/globals/%.cpp $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/hexapod/%.cpp $(HEXA_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(PARAMS) $(GLOBALS) $(HEXA_DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

test_ikinematics: $(TEST_IKINEMATICS_OBJ)
	@echo "Compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
