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

#### Global Includes
_GLOBALS = axi_ip_globals.h global_defines.h linux_timing.h
GLOBALS  = $(patsubst %,$(IDIR)/globals/%,$(_GLOBALS))

_GLOBALS_OBJ = axi_ip_globals.o linux_timing.o
GLOBALS_OBJ  = $(patsubst %,$(ODIR)/%,$(_GLOBALS_OBJ))

#### Global & Hexapod Parameters
_PARAMS = hexapod_ip_params.h hexapod_params.h ip_reg_map.h
PARAMS  = $(patsubst %,$(IDIR)/params/%,$(_PARAMS))

#### Hexapod Objects
_HEXA_DEPS = hexapod_kinematics.h hexapod_locomotion.h
HEXA_DEPS  = $(patsubst %,$(IDIR)/hexapod/%,$(_HEXA_DEPS))

_HEXA_OBJ = hexapod_kinematics.o hexapod_locomotion.o
HEXA_OBJ  = $(patsubst %,$(ODIR)/%,$(_HEXA_OBJ))

#### I2C Objects
_I2C_DEPS	= i2c_ip_context.h
I2C_DEPS 	= $(patsubst %,$(IDIR)/ip_context/%,$(_I2C_DEPS))

_I2C_OBJ 	= i2c_ip_context.o
I2C_OBJ  	= $(patsubst %,$(ODIR)/%,$(_I2C_OBJ))

#### I2C OLED Objects
_I2C_OLED 	= i2c_oled.h i2c_oled_params.h
I2C_OLED 	= $(patsubst %,$(IDIR)/i2c_peripherals/%,$(_I2C_OLED))

_I2C_OLED_OBJ	= i2c_oled.o
I2C_OLED_OBJ  	= $(patsubst %,$(ODIR)/%,$(_I2C_OLED_OBJ))

#### Test Objects
_LOCOMOTION_OBJ = locomotion.o 
LOCOMOTION_OBJ  = $(patsubst %,$(ODIR)/%,$(_LOCOMOTION_OBJ))

$(info > Globals)
$(info $(GLOBALS))

$(info > Hexapod Class Dependencies)
$(info $(HEXA_DEPS))

$(info > Parameters)
$(info $(PARAMS))

$(info > Hexapod Objects)
$(info $(HEXA_OBJ))

$(info > I2C Includes)
$(info $(I2C_OBJ))

$(info > I2C OLED Includes)
$(info $(I2C_OLED_OBJ))

$(info > Test Objects)
$(info $(LOCOMOTION_OBJ))

$(ODIR)/%.o: $(SDIR)/globals/%.cpp $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/hexapod/%.cpp $(HEXA_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/ip_context/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)
	
$(ODIR)/%.o: $(SDIR)/i2c_peripherals/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/hexapod_run/%.cpp $(PARAMS) $(GLOBALS) $(HEXA_DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

hexapod_run_locomotion:  $(LOCOMOTION_OBJ) $(I2C_OLED_OBJ) $(I2C_OBJ) $(HEXA_OBJ) $(GLOBALS_OBJ)
	@echo "==============================="
	@echo -e "Compile \033[92m $(ODIR)/$@ \033[0m"
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)*~

