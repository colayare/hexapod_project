import os, mmap, struct

################################################################################
#### Inverse Kinematics IP Memory Map
################################################################################
class axi_ip_mmap(object):
    ############################################################################
    #### Properties
    ############################################################################
    ## AXI Parameters
    __base_address  = 0x40000000    # AXI IP Base Address
    __slot_size     = 0x1000        # AXI Slot Size
    __axi_word_size = 4             # AXI Word Size in bytes
    ## AXI Memory Map
    axi_map         = None          # AXI Memory Map Pointer
    ## Log File Handling
    enable_ip_logs  = False         # Enable log generation
    ip_logfile_path = ''            # Log file Path
    log_file        = ''            # Log file content
    
    @property
    def base_address(self):
        return self.__base_address
        
    @property
    def slot_size(self):
        return self.__slot_size
    
    @property
    def word_size(self):
        return self.__axi_word_size
        
    ############################################################################
    #### Methods
    ############################################################################
    ## Constructor
    def __init__(self, base_address=0x40000000, slot_size=0x1000, enable_ip_logs=False):
        super(axi_ip_mmap, self).__init__()
        self.__base_address     = base_address
        self.__slot_size        = slot_size
        self.enable_ip_logs     = enable_ip_logs
        if ( self.enable_ip_logs ):
            print('AXI IP '+str(self)+' > Enable log.')
        self.init_axi_map()
        return None
        
    #### Format ################################################################
    #### Format to hex string
    def int_to_hexstr(self, number):
        return str(hex(number)[2:].zfill(8)).rstrip("L")
    
    #### Set IP Parameters #####################################################
    ## Set AXI IP word size in bytes
    def set_axi_word_size(self, word_size):
        self.__axi_word_size = word_size
        return True
    
    ## Set AXI IP base address
    def set_base_address(self, base_address):
        self.__base_address = base_address
        return True
    
    ## Set AXI IP Slot Size
    def set_slot_size(self, slot_size):
        self.__slot_size = slot_size
        return True
    
    #### Memory Map ############################################################
    ## Initialize AXI IP Memory Map
    def init_axi_map(self):
        fd = os.open("/dev/mem", os.O_RDWR|os.O_SYNC)
        self.axi_map = mmap.mmap(fd, 
                      self.__slot_size,
                      mmap.MAP_SHARED,
                      prot = mmap.PROT_READ | mmap.PROT_WRITE,
                      offset = self.__base_address)
        os.close(fd)
        return True
    
    ## Set Pointer to Memory Map
    def set_ptr(self, axi_word):
        self.axi_map.seek(axi_word*self.__axi_word_size)
        return True
    
    #### AXI Read : Default AXI Read Operation
    ## Inputs:
    ## > address     : 32-bit int
    ## Outputs:
    ## > read value : 32-bit int
    def axi_read(self, address):
        self.set_ptr(address)
        read_val = self.to_int(self.axi_map.read(self.__axi_word_size))
        if ( self.enable_ip_logs ):
            self.log_file += 'R['+self.int_to_hexstr(address)+'] = '+self.int_to_hexstr(read_val)+'\n'
        return read_val
    
    #### AXI Read Hex : AXI Read with hex string return value
    ## Inputs:
    ## > address        : 32-bit int
    ## Outputs:
    ## > read value     : 32-bit hex string
    def axi_hread(self, address):
        self.set_ptr(address)
        read_val_int = self.to_int(self.axi_map.read(self.__axi_word_size))
        read_val_hex = hex(read_val_int)[2:].rstrip("L").zfill(8)
        if ( self.enable_ip_logs ):
            self.log_file += 'R['+self.int_to_hexstr(address)+'] = '+read_val_hex+'\n'
        return read_val_hex
        
    #### AXI Fast Read Hex : AXI Fast Read Operation, doesn't generate log
    ## Inputs:
    ## > address        : 32-bit int
    ## Outputs:
    ## > read value     : 32-bit int
    def axi_fread(self):
        return self.to_int(self.axi_map.read(self.__axi_word_size))
        
    #### AXI Read Mask : AXI Read Operation with output mask
    ## Inputs:
    ## > address     : 32-bit int
    ## > mask        : 32-bit int
    ## Outputs:
    ## > read value : 32-bit int
    def axi_read_mask(self, address, mask):
        self.set_ptr(address)
        read_val = self.to_int(self.axi_map.read(self.__axi_word_size)) & mask
        if ( self.enable_ip_logs ):
            self.log_file += 'R['+self.int_to_hexstr(address)+'] = '+self.int_to_hexstr(read_val)+','+self.int_to_hexstr(mask)+'\n'
        return read_val
    
    #### AXI Write : Default AXI Write Operation
    ## Inputs:
    ## > address      : 32-bit int
    ## > write value  : 32-bit int
    ## Outputs:
    ## > True
    def axi_write(self, address, value):
        self.set_ptr(address)
        self.axi_map.write(self.to_bytes(value))
        if ( self.enable_ip_logs ):
            self.log_file += 'W['+self.int_to_hexstr(address)+'] = '+self.int_to_hexstr(value)+'\n'
        return True
        
    #### AXI Write : AXI Write Operation with hex string input
    ## Inputs:
    ## > address      : 32-bit int
    ## > write value  : 32-bit hex string
    ## Outputs:
    ## > True
    def axi_hwrite(self, address, value):
        value = int(value, 16)
        self.set_ptr(address)
        self.axi_map.write(self.to_bytes(value))
        if ( self.enable_ip_logs ):
            self.log_file += 'W['+self.int_to_hexstr(address)+'] = '+self.int_to_hexstr(value)+'\n'
        return True
    
    #### AXI Fast Write : AXI Fast Write Operation, does not generate log
    ## Inputs:
    ## > address      : 32-bit int
    ## > write value  : 32-bit int
    ## Outputs:
    ## > True
    def axi_fwrite(self, value):
        self.axi_map.write(self.to_bytes(value))
        return True
    
    #### AXI Write Mask : AXI Write operation with mask
    ## Inputs:
    ## > address      : 32-bit int
    ## > write value  : 32-bit int
    ## > mask         : 32-bit int
    ## Outputs:
    ## > True
    def axi_write_mask(self, address, value, mask):
        self.set_ptr(address)
        read_val  = self.to_int(self.axi_map.read(self.__axi_word_size))
        write_val = (value & mask) + (read_val & (mask ^ 0xFFFFFFFF))
        self.set_ptr(address)
        self.axi_map.write(self.to_bytes(write_val))
        if ( self.enable_ip_logs ):
            self.log_file += 'WM['+self.int_to_hexstr(address)+'] = '+self.int_to_hexstr(write_val)+','+self.int_to_hexstr(mask)+'\n'
        return True
    
    #### Data Conversion Functions #############################################
    ## Bytes data type to Integer
    def to_int(self, bytes_in):
        return struct.unpack("<HH", bytes_in)[0]+(struct.unpack("<HH", bytes_in)[1]<<16)
    
    ## Integer to Bytes
    def to_bytes(self, int_in):
        return struct.pack("<I", int_in)
    
    #### Log Handling ##########################################################
    ## Export IP Transactions Log
    def export_log(self):
        print('AXI IP '+str(self)+' > Exporting log to '+self.ip_logfile_path)
        f = open(self.ip_logfile_path, "w")
        f.write(self.log_file)
        f.close()
        return True
    
    ## Clean Log File
    def clean_log(self):
        self.log_file = ''
        return True
    
    #### Aditional Functions ###################################################
    ## Display all slot registers 
    def show_regs(self, start_address=0, end_address=0x1000, autoskip=True):
        print('#'*20+'\nAXI '+str(self)+' REGS :')
        for i in range (start_address, end_address+1):
            address = i
            read_val = self.axi_hread(address)
            print('R['+str(hex(address)[2:].zfill(8))+'] = '+str(read_val[2:].zfill(8)))
        if ( not autoskip ):
            raw_input()
        return None