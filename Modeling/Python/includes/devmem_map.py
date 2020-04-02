import os, mmap, struct

###############################################################################
#### Inverse Kinematics IP Memory Map
###############################################################################
class ikinematics_mmap(object):
    ###########################################################################
    #### Properties
    ###########################################################################
    ## AXI Parameters
    __base_address  = 0x40000000
    __slot_size     = 0x1000
    __axi_word_size = 4
    ## Log File Handling
    gen_log_enable  = False
    ip_logfile_path = None
    __log_file      = ''
    
    @property
    def base_address(self):
        return self.__base_address
        
    @property
    def slot_size(self):
        return self.__slot_size
    
    @property
    def word_size(self):
        return self.__axi_word_size
        
    ###########################################################################
    #### Methods
    ###########################################################################
    ## Constructor
    def __init__(self, base_address=0x40000000, slot_size=0x1000, gen_log_enable=False):
        self.__base_address     = base_address
        self.__slot_size        = slot_size
        self.gen_log_enable     = gen_log_enable
        if ( self.gen_log_enable ):
            print('AXI IP '+str(self)+' enable log.')
        self.init_axi_map()
        return None
    
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
    
    ## AXI Read
    def axi_read(self, address):
        self.set_ptr(address)
        read_val_int = self.to_int(self.axi_map.read(self.__axi_word_size))
        read_val_hex = hex(read_val_int).rstrip("L")
        if ( self.gen_log_enable ):
            self.__log_file += 'R['+hex(address)[2:]+'] = '+str(read_val_hex)+'\n'
        return read_val_hex
    
    ## AXI Write
    def axi_write(self, address, value):
        if ( type(value) is str ):
            value = int(value, 16)
        self.set_ptr(address)
        self.axi_map.write(self.to_bytes(value))
        if ( self.gen_log_enable ):
            self.__log_file += 'W['+hex(address)[2:]+'] = '+str(hex(value)[2:])+'\n'
        return True
    
    #### Data Conversion Functions
    ## Bytes data type to Integer
    def to_int(self, bytes_in):
        return struct.unpack("<HH", bytes_in)[0]+(struct.unpack("<HH", bytes_in)[1]<<16)
    
    ## Integer to Bytes
    def to_bytes(self, int_in):
        return struct.pack("<I", int_in)
    
    ## Display all slot registers 
    def show_regs(self, from_address=0x40000000, to_address=0x40):
        for i in range (int(to_address-from_address)):
            address = from_address+i
            read_val = hex(self.to_int(self.axi_read(address)))
            print('R['+str(hex(i))+'] = '+str(hex(read_val)))
        return None
    
    #### Log Handling
    ## Export IP Transactions Log
    def export_log(self):
        f = open(self.ip_logfile_path, "w")
        f.write(self.__log_file)
        f.close()
        return True
    
    ## Clean Log File
    def clean_log(self):
        self.__log_file = ''
        return True