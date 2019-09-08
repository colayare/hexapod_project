# -*- coding: utf-8 -*-
"""
Created on Thu Sep  5 19:46:04 2019

@author: Roy

Python2.7
"""

from ctypes import *

class numeric_conversions:
    ################
    ### Properties
    ################
    fxp_width   = 20    # Fixed Point Number Width
    fxp_int     = 4     # Fixed Point Number of integer bits
    flp_width   = 32    # Floating Point Number Width
    
    ### Constructor
    def __init__(self, fxp_width=20, fxp_int=4, flp_width=32):
        self.fxp_width  = fxp_width
        self.fxp_int    = fxp_int
        self.flp_width  = flp_width
        return None
    
    ##################
    ### Functions
    ##################
    # Cast types
    def hexa_str_in(hexa_num):
        return hex(hexa_num)
    
    #### hfix2float
    # Converts num from str hex to decimal float
    def hfix2dfloat(self, num):
        binNum = bin(int(num,16))[2:].zfill(self.fxp_width)
        intBin = binNum[0:self.fxp_int]
        sign = intBin[0]
        if (sign=='1'):
            intVal = -(2**(self.fxp_int) - int(intBin,2))
        else:
            intVal = int(intBin,2)
        floatVal = 0
        for i in range(self.fxp_width-self.fxp_int):
            if(binNum[self.fxp_int+i:self.fxp_int+1+i]=='1'):
                floatVal += 2**(-i-1)
        return intVal+floatVal
    
    #### dfloat2hfix
    # Convertds decimal float number to str hexa
    def dfloat2hfix(self, num):
        intSize = self.fxp_int
        intPart = int(abs(num))
        floatPart = abs(num) - intPart
        if (num>=0):
            if (intPart==0):
                intVal = '0' * intSize
            else:
                intVal = '0'+bin(intPart)[2:intSize+1].zfill(intSize-1)
        else:
            if (intPart==0):
                intVal = '1' * intSize
                floatPart = 1-floatPart
            else:
                if (floatPart==0):
                    offset = 0
                else:
                    offset = 1
                intVal = ''+bin((2**intSize)-intPart-offset)[2:].zfill(intSize)
                floatPart = 1-floatPart
        floatVal = ''
        for i in range(self.flp_width-self.fxp_width):
            if (floatPart >= 2**(-i-1)):
                floatVal += '1'
                floatPart -= 2**(-i-1)
            else:
                floatVal += '0'
        binOut = intVal+floatVal
        result = ''+hex(int(binOut,2))[2:].zfill(int(self.fxp_width/4))
        return result
    
    #### hfloat2dfloat
    # Convert Hexa Float number to Decimal Float
    def hfloat2dfloat(self, num):
        int_in  = int(num, 16)
        c_ptr   = pointer(c_int(int_in))
        flp     = cast(c_ptr, POINTER(c_float))
        return flp.contents.value
    
    #### dfloat2hfloat
    def dfloat2hfloat(self, num):
        flp_in  = num
        c_ptr   = pointer(c_float(flp_in))
        hexflp  = cast(c_ptr, POINTER(c_int))
        return hex(hexflp.contents.value)[2:]