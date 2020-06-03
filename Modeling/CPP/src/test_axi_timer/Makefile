# Includes Directory
IDIR=$(MODEL_CPP_PATH)/includes
SDIR=$(MODEL_CPP_PATH)/src
# Execution directory set as Outputs Directory
ODIR=$(MODEL_CPP_PATH)/bin
$(info ===============================)
$(info Includes dir $(IDIR))
$(info Sources dir $(SDIR))
$(info Output dir $(ODIR))
$(info ===============================)

#### Compilation Parameters
CC=g++
CFLAGS=-I$(IDIR) 

#### 
_GLOBALS = axi_ip_globals.h global_defines.h linux_timing.h
GLOBALS  = $(patsubst %,$(IDIR)/globals/%,$(_GLOBALS))

_GLOBALS_OBJ = axi_ip_globals.o linux_timing.o
GLOBALS_OBJ  = $(patsubst %,$(ODIR)/%,$(_GLOBALS_OBJ))

_PARAMS = hexapod_ip_params.h hexapod_params.h ip_reg_map.h
PARAMS  = $(patsubst %,$(IDIR)/params/%,$(_PARAMS))

_HEXA_DEPS = hexapod_kinematics.h hexapod_locomotion.h
HEXA_DEPS  = $(patsubst %,$(IDIR)/hexapod/%,$(_HEXA_DEPS))

_HEXA_OBJ = hexapod_kinematics.o hexapod_locomotion.o
HEXA_OBJ  = $(patsubst %,$(ODIR)/%,$(_HEXA_OBJ))

_TEST_AXI_TIMER_OBJ = test_axi_timer.o 
TEST_AXI_TIMER_OBJ  = $(patsubst %,$(ODIR)/%,$(_TEST_AXI_TIMER_OBJ))

$(info > Globals)
$(info $(GLOBALS))

$(info > Parameters)
$(info $(PARAMS))

$(info > Test Objects)
$(info $(TEST_AXI_TIMER_OBJ))

$(info > Hexapod Class Dependencies)
$(info $(HEXA_DEPS))

$(info > Hexapod Objects)
$(info $(HEXA_OBJ))

$(ODIR)/%.o: $(SDIR)/globals/%.cpp $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/hexapod/%.cpp $(HEXA_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/test_axi_timer/%.cpp $(PARAMS) $(GLOBALS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

test_axi_timer:  $(TEST_AXI_TIMER_OBJ) $(GLOBALS_OBJ) $(HEXA_OBJ)
	@echo "==============================="
	@echo -e "Compile \033[92m $(ODIR)/$@ \033[0m"
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)*~

