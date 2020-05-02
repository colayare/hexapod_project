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


#### I2C OLED
_I2C_OLED 	= i2c_oled.h i2c_oled_params.h
I2C_OLED 	= $(patsubst %,$(IDIR)/i2c_peripherals/%,$(_I2C_OLED))

_I2C_OLED_OBJ	= i2c_oled.o
I2C_OLED_OBJ  	= $(patsubst %,$(ODIR)/%,$(_I2C_OLED_OBJ))

$(info > I2C Includes)
$(info $(I2C_OLED))

#### Test Objects
_TEST_OLED_OBJ	= test_oled.o
TEST_OLED_OBJ  = $(patsubst %,$(ODIR)/%,$(_TEST_OLED_OBJ))

$(info > Test Object)
$(info $(TEST_OLED_OBJ))

$(ODIR)/%.o: $(SDIR)/globals/%.cpp $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/ip_context/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)
	
$(ODIR)/%.o: $(SDIR)/i2c_peripherals/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS) 
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/test_oled/%.cpp $(I2C_DEPS) $(PARAMS) $(GLOBALS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)
	
test_oled:  $(TEST_OLED_OBJ) $(I2C_OLED_OBJ) $(I2C_OBJ) $(GLOBALS_OBJ)
	@echo "==============================="
	@echo -e "Compile \033[92m $(ODIR)/$@ \033[0m"
	$(CC) -Wall -o $(ODIR)/$@ $^ $(CFLAGS)
	
clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)*~