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

$(info > Globals)
$(info $(GLOBALS))

#### Parameter Includes
_PARAMS = i2c_ip_defines.h ip_reg_map.h
PARAMS  = $(patsubst %,$(IDIR)/params/%,$(_PARAMS))

$(info > Parameter Includes)
$(info $(PARAMS))

#### I2c IP Includes
_I2C_DEPS	= i2c_ip_context.h
I2C_DEPS 	= $(patsubst %,$(IDIR)/ip_context/%,$(_I2C_DEPS))

_I2C_OBJ 	= i2c_ip_context.o
I2C_OBJ  	= $(patsubst %,$(ODIR)/%,$(_I2C_OBJ))

$(info > I2C Includes)
$(info $(I2C_DEPS))

#### Test Objects
_TEST_I2C_OBJ	= test_i2c.o
TEST_I2C_OBJ  = $(patsubst %,$(ODIR)/%,$(_TEST_I2C_OBJ))

$(info > Test Object)
$(info $(TEST_I2C_OBJ))

$(info ===============================)

$(ODIR)/%.o: $(SDIR)/globals/%.cpp $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/ip_context/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/test_i2c/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)
	
test_i2c:  $(TEST_I2C_OBJ) $(I2C_OBJ) $(GLOBALS_OBJ)
	@echo "Compile $(ODIR)/$@ "
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)*~