import os

class axi_ikinematics_ip:
    base_address = 0x40000000
    
    def __init__(self):
        return None

    def axi_write(self, address, value):
        addr = str( hex(self.base_address+address*4) )
        value = hex(value)[2:].zfill(8)[0:8]
        cmd  = "devmem "+addr+' w 0x'+value
        os.system(cmd)
        return None
    
    def axi_read(self, address):
        addr = str( hex(self.base_address+address*4))
        cmd  = 'devmem '+addr
        return os.popen(cmd).read()[:-1]
    
    def write_fifo(self):
        os.system('devmem 0x40000000 w 0x00000001')
        os.system('devmem 0x40000000 w 0x00000000')
        return True
    
    def trigger_iKinematics(self):
        os.system('devmem 0x40000000 w 0x00000002')
        os.system('devmem 0x40000000 w 0x00000000')
        return True