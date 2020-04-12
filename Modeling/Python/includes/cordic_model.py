# -*- coding: utf-8 -*-
"""
Created on Mon Apr  6 22:03:10 2020

@author: Roy
"""

import numpy as np
from numeric_conversions import numeric_conversions as numeric_conversions

class COEFF(numeric_conversions):
    __h_fxp_coeffs = np.array([ '11b6e',    # -1 
                                '0c90f',    # 0
                                '076b1',    # 1
                                '03eb6',    # 2
                                '01fd5',    # 3
                                '00ffa',    # 4
                                '007ff',    # 5
                                '003ff',    # 6
                                '001ff',    # 7
                                '000ff',    # 8
                                '0007f',    # 9
                                '0003f',    # 10
                                '0001f',    # 11
                                '0000f',    # 12
                                '00007',    # 13
                                '00003',    # 14
                                '00001'])   # 15
    
    __iterations = np.array(range(-1,16))
    
    @property
    def coeffs_size(self):
        return self.__h_fxp_coeffs.size
    
    @property
    def fxp_hcoeffs(self):
        return self.__h_fxp_coeffs
    
    @property
    def flp_hcoeffs(self):
        dcoeffs = np.array([])
        for coeff in self.dcoeffs:
            dcoeffs = np.append(dcoeffs, self.dfloat2hfloat(coeff))
        return dcoeffs
    
    @property
    def dcoeffs(self):
        dcoeffs = np.array([])
        for coeff in self.__h_fxp_coeffs:
            dcoeffs = np.append(dcoeffs, self.hfix2dfloat(coeff))
        return dcoeffs
    
    def dcoeff(self, i):
        return self.dcoeffs[np.where(self.__iterations == i)[0][0]]
    
class CORDIC(COEFF):
#    __coeff_ptr_offset = 1
    negative_iterations = 0
    positive_iterations = 16
    expanded    = False
    operator    = 'circular'
    mode        = 'rotation'
    __input     = np.array([0, 0, 0])
    __output    = np.array([0, 0, 0])
    
    @property
    def x(self):
        return self.__input[0]
    @property
    def y(self):
        return self.__input[1]
    @property
    def z(self):
        return self.__input[2]
    
    @property
    def xo(self):
        return self.__output[0]
    @property
    def yo(self):
        return self.__output[1]
    @property
    def zo(self):
        return self.__output[2]
    
    @property
    def out(self):
        return self.__output
            
    def coeff(self, i):
        if ( self.operator == 'circular' ):
            return self.dcoeff(i)
        else:
            return 2**(-i)
    
    def __init__(self, operator, mode, negative_iterations, positive_iterations, expanded=False):
        self.expanded            = expanded
        self.negative_iterations = negative_iterations
        self.positive_iterations = positive_iterations
        self.operator = operator
        self.mode = mode
        return None
    
    def calculate(self, x, y, z):
        self.__input = np.array([x, y, z])
        xr      = x
        yr      = y
        zr      = z
        for i in range ( self.negative_iterations, self.positive_iterations ):
            #### Sign Calculation
            if ( self.mode == 'rotational' ):
                d = 1 if zr >= 0 else -1
            else:
                if ( self.operator == 'circular' ):
                    d = -1 if yr >= 0 else 1
                else:
                    d = np.sign(xr) * np.sign(yr)
            #### CORDIC Iterations
            if ( self.operator == 'circular' ):
                xn = xr - (d * yr * 2.0**(-i))
                yn = yr + (d * xr * 2.0**(-i))
                zn = zr - (d * self.coeff(i))
            elif (self.operator == 'hyperbolic' ):
                zn = 0
                if ( i <= 0 ):
                    xn = xr - ( d * yr * (1 - 2**(i-2)) )
                    yn = -1 * ( d * xr * (1 - 2**(i-2)) ) + yr
                else:
                    xn = xr - ( d * yr * 2**(-i) )
                    yn = -1 * ( d * xr * 2**(-i) ) + yr
            else:
                xn = xr
                yn = yr - ( d * xr * 2**(-i) )
                zn = zr + ( d * 2**(-i) )
            xr, yr, zr = xn, yn, zn
        self.__output = xn, yn, zn
        return xn, yn, zn

#a = 0.03
#b = 0
#C3_CV = CORDIC('circular', 'vectorial', -1, 14, expanded=True)
#C3_CV.calculate()