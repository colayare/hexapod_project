# Includes Directory
IDIR=$(MODEL_CPP_PATH)/includes
SDIR=$(MODEL_CPP_PATH)/src/test_ikinematics_ip
# Execution directory set as Outputs Directory
ODIR=$(PWD)/outputs
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


_GLOBALS   = axi_ip_globals.hpp 
GLOBALS    = $(patsubst %,$(IDIR)/globals/%,$(_DEPS))

_HEXA_DEPS 	= hexapod_kinematics.hpp
HEXA_DEPS	= $(patsubst %,$(IDIR)/hexapod/%,$(_HEXA_DEPS))

_PARAMS = hexapod_ip_params.hpp hexapod_params.hpp
PARAMS  = $(patsubst %,$(IDIR)/params/%,$(_PARAMS))

_TEST_IKINEMATICS_OBJ 	= test_ikinematics.o
TEST_IKINEMATICS_OBJ    = $(patsubst %,$(ODIR)/%,$(_TEST_IKINEMATICS_OBJ))

$(info $(GLOBALS))
$(info $(HEXA_DEPS))
$(info $(PARAMS))
$(info $(TEST_IKINEMATICS_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(PARAMS) $(GLOBALS) $(HEXA_DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

test_ikinematics: $(TEST_IKINEMATICS_OBJ)
	@echo "Compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)