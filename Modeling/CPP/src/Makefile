# Includes Directory
IDIR=$(MODEL_CPP_PATH)/includes/globals
PDIR=$(MODEL_CPP_PATH)/includes/params
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


_DEPS   = axi_ip_globals.hpp data_types.hpp ikinematics_ip.hpp
DEPS    = $(patsubst %,$(IDIR)/%,$(_DEPS))

_PARAMS = ikinematics_params.hpp
PARAMS  = $(patsubst %,$(PDIR)/%,$(_PARAMS))

_TEST_IKINEMATICS_OBJ = test_ikinematics.o
TEST_IKINEMATICS_OBJ    = $(patsubst %,$(ODIR)/%,$(_TEST_IKINEMATICS_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) $(PARAMS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

test_ikinematics: $(TEST_IKINEMATICS_OBJ)
	@echo "Compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
    