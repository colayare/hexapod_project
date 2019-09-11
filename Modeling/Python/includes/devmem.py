import os

class axi_ikinematics_ip:
    __base_address = 0x40000000
    
    def __init__(self, base_address=0x40000000):
        self.__base_address = base_address
        self.__str_base_address = str( hex(self.__base_address) )
        return None

    def axi_write(self, address, value):
        addr = str( hex(self.__base_address+address*4) )
        value = hex(value)[2:].zfill(8)[0:8]
        cmd  = "devmem "+addr+' w 0x'+value
        os.system(cmd)
        return None
    
    def axi_read(self, address):
        addr = str( hex(self.__base_address+address*4))
        cmd  = 'devmem '+addr
        return os.popen(cmd).read()[:-1]
    
    def write_fifo(self):
        os.system('devmem '+self.__str_base_address+' w 0x00000001')
        os.system('devmem '+self.__str_base_address+' w 0x00000000')
        return True
    
    def trigger_iKinematics(self):
        os.system('devmem '+self.__str_base_address+' w 0x00000002')
        os.system('devmem '+self.__str_base_address+' w 0x00000000')
        return True
    
    def set_hexapod_leg(self, leg_idx):
        value = hex(leg_idx)[2:].zfill(8)[0:8]
        addr = str( hex(self.__base_address+4) )
        os.system('devmem '+addr+' w '+value )
        os.system('devmem '+self.__str_base_address+' w 0x00000002' )
        return True
    
    def explore_pwm(self):
        for i in range (18):
            print(i, self.axi_read(0x17+i))
        return True