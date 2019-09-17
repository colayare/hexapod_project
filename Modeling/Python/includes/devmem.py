import os

class axi_ikinematics_ip:
    __base_address = 0x40000000
    __base_address_str = str(hex(0x40000000))
    
    def get_address(self, relative_address, return_type="str"):
        absolute_address = self.__base_address + relative_address * 4
        if ( return_type == "int" ):
            return absolute_address
        elif ( return_type == "hex" ):
            return hex(absolute_address)
        else:
            return str(hex(absolute_address))
    
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
    
    def explore_pwm(self):
        for i in range (18):
            print(i, self.axi_read(0x17+i))
        return True