import os, mmap, struct

class ikinematics_mmap:
    __base_address = 0x40000000
    __page_size = 0x1000
    __axi_word_size = 4

    def __init__(self, base_address=0x40000000, page_size=0x1000):
        self.__base_address = base_address
        self.__page_size = page_size
        self.init_axi_map()
        return None
    
    def set_axi_word_size(self, word_size):
        self.__axi_word_size = word_size
        return True
    
    def set_base_address(self, base_address):
        self.__base_address = base_address
        return True
    
    def set_page_size(self, page_size):
        self.__page_size = page_size
        return True
    
    def init_axi_map(self):
        fd = os.open("/dev/mem", os.O_RDWR|os.O_SYNC)
        self.axi_map = mmap.mmap(fd, 
                      self.__page_size,
                      mmap.MAP_SHARED,
                      prot = mmap.PROT_READ | mmap.PROT_WRITE,
                      offset = self.__base_address)
        os.close(fd)
        return True
    
    def set_ptr(self, axi_word):
        self.axi_map.seek(axi_word*self.__axi_word_size)
        return True
    
    def axi_read(self, address):
        self.set_ptr(address)
        return hex(self.to_int(self.axi_map.read(self.__axi_word_size))).rstrip("L")
    
    def axi_write(self, address, value):
        if ( type(value) is str ):
            value = int(value, 16)
        self.set_ptr(address)
        self.axi_map.write(self.to_bytes(value))
        return True
    
    def to_int(self, bytes_in):
        return struct.unpack("<HH", bytes_in)[0]+(struct.unpack("<HH", bytes_in)[1]<<16)

    def to_bytes(self, int_in):
        return struct.pack("<I", int_in)
    
    def show_regs(self, from_address=0x40000000, to_address=0x40):
        for i in range (int(to_address-from_address)):
            address = from_address+i
            read_val = hex(self.to_int(self.axi_read(address)))
            print(read_val)
        return None