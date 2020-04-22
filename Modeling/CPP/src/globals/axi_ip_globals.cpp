#include<iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include "globals/global_defines.h"
#include "globals/axi_ip_globals.h"

//== Init AXI IP Memory Map
int32_t ip_context::init_axi_mmap_ptr(int32_t axi_mmap_size, int32_t axi_base_address, int32_t axi_word_size) {
    int _fdmem;
    int *axi_mmap_ptr = NULL;
    const char memDevice[] = "/dev/mem";
    
    //== Validate 
    if ( !( axi_mmap_size && axi_base_address ) ) {
        cout << "ERROR : AXI MMAP Size or Base Address is zero" << endl;
        return 0;
    }

    _fdmem = open( memDevice, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        cout << "Failed to open /dev/mem !\n" << endl;
        return 0;
    } else {
        cout << "Open /dev/mem successfully !\n" << endl;
    }
    
    axi_mmap_ptr = (int *)(mmap(0, axi_mmap_size*axi_word_size, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, axi_base_address));
    
    // Setting Class members values
    this->_axi_mmap_ptr      = axi_mmap_ptr;
    this->_axi_mmap_size     = axi_mmap_size;
    this->_axi_base_address  = axi_base_address;
    this->_axi_word_size     = axi_word_size;
    
    return 1;
}
//== Get Memory Map Pointer
int32_t *ip_context::get_mmap_ptr() { return this->_axi_mmap_ptr; }
//== AXI IP Read
int32_t ip_context::axi_read(int32_t read_address) {
    #ifdef __IP_LOG
    int32_t val_print = *(this->_axi_mmap_ptr+read_address);
    cout << "R[" << read_address << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
int32_t ip_context::axi_read_mask(int32_t read_address, int32_t mask) {
    #ifdef __IP_LOG
    int32_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    cout << "RM[" << read_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write(int32_t write_address, int32_t value) {
    #ifdef __IP_LOG
    cout << "W[" << write_address << "] = " << hex << value << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask(int32_t write_address, int32_t value, int32_t mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    int32_t val_print = register_value + (value & mask);
    cout << "WM[" << write_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set(int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    int32_t val_print = register_value + bit_mask;
    cout << "BS[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr(int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    int32_t val_print = register_value + bit_mask;
    cout << "BC[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs(int32_t start_address, int32_t end_address) {
    //TODO: Check counter i word size access
    for (int32_t i=0;i<(end_address-start_address);i++) {
        cout << "REG[" << i << "]\t= " << hex << *(this->_axi_mmap_ptr+i) << endl;
    }
}