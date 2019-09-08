# -*- coding: utf-8 -*-
"""
Created on Sun Sep  1 21:06:41 2019

@author: Roy
"""
import sys
sys.path.append('./includes')
from numeric_conversions import numeric_conversions as nc
import kinematics as ki

def addroff(axi_address):
    baseaddr = 0x40000000
    return str(hex(baseaddr+axi_address*4))

fp = nc()

write_fifo = '0'*7+'1'
trigger_ik = '0'*7+'2'
clear_axir = '0'*8


### Hex x, y, z
hx_in = "3dfdf3b6"
hy_in = "3d4ccccc"
hz_in = "bdd70a3d"

dx_in = fp.hfloat2dfloat(hx_in)
dy_in = fp.hfloat2dfloat(hy_in)
dz_in = fp.hfloat2dfloat(hz_in)

print('devmem '+addroff(2)+' w 0x'+hx_in)
print('devmem '+addroff(3)+' w 0x'+hy_in)
print('devmem '+addroff(4)+' w 0x'+hz_in)

print('devmem '+addroff(0)+' w 0x'+write_fifo)
print('devmem '+addroff(0)+' w 0x'+clear_axir)
print('devmem '+addroff(0)+' w 0x'+trigger_ik)
print('devmem '+addroff(0)+' w 0x'+clear_axir)

print('devmem '+addroff(2))
print('devmem '+addroff(3))
print('devmem '+addroff(4))

#[dqx, dqy, dqz] = ki.iKinematics(dx_in, dy_in, dz_in)
#hqx = fp.dfloat2hfloat(dqx)
#hqy = fp.dfloat2hfloat(dqy)
#hqz = fp.dfloat2hfloat(dqz)
#
#print(dqx, dqy, dqz)
#print(hqx, hqy, hqz)