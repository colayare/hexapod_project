/*
 * ip_context.cpp
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#include "ip_context.h"
#define __IP_LOG

uint32_t ip_context::axi_init(uint32_t axi_base_address, uint32_t axi_mmap_size, uint32_t axi_word_size) {
  this->_axi_mmap_ptr     = (volatile uint32_t *) axi_base_address;
  this->_axi_mmap_size    = axi_mmap_size;
  this->_axi_base_address = axi_base_address;
  this->_axi_word_size    = axi_word_size;

  #ifdef __IP_LOG
  std::cout << "Initialized IP" << std::endl;
  std::cout << "MMAP PTR  : " << std::hex << this->_axi_mmap_ptr;
  std::cout << "MMAP ADDR : " << std::hex << this->_axi_base_address << std::endl;
  std::cout << "MMAP SIZE : " << this->_axi_mmap_size << std::hex << std::endl;
  std::cout << "MMAP WORD : " << this->_axi_word_size << std::hex << std::endl;
  #endif
  return 1; 
}

uint32_t ip_context::axi_read(uint32_t read_address) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address);
    std::cout << "R32[" << read_address << "] = " << std::hex << val_print << std::endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
uint32_t ip_context::axi_read_mask(uint32_t read_address, uint32_t mask) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    std::cout << "RM32[" << read_address << "," << mask << "] = " << std::hex << val_print << std::endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write(uint32_t write_address, uint32_t value) {
    #ifdef __IP_LOG
    std::cout << "W32[" << write_address << "] = " << std::hex << value << std::endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + (value & mask);
    std::cout << "WM32[" << write_address << "," << mask << "] = " << std::hex << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    std::cout << "BS32[" << address << "," << bit_mask << "] = " << std::hex << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    std::cout << "BC32[" << address << "," << bit_mask << "] = " << std::hex << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs(uint32_t start_address, uint32_t end_address) {
    //TODO: Check counter i word size access
    for (uint32_t i=0;i<(end_address-start_address);i++) {
        std::cout << "REG32[" << i+start_address << "]\t= " << std::hex << *(this->_axi_mmap_ptr+start_address+i) << std::endl;
    }
}
//== AXI IP Read Expected Value
uint32_t ip_context::axi_read_exp(uint32_t address, uint32_t expected_value) {
    uint32_t result;
    if ( *(this->_axi_mmap_ptr+address) == expected_value ) {
        result =  1;
    } else {
        
        result = 0;
    }
    #ifdef __IP_LOG
    std::cout << "RE32[" << std::hex << address << "," << expected_value << "] = " << *(this->_axi_mmap_ptr+address) << " = " << result << std::endl;
    #endif
    return result;
}
//== AXI IP Wait for a Read Value
uint32_t ip_context::axi_wait(uint32_t address, uint32_t value, uint32_t cycles) {
    uint32_t cnt    = 0;
    uint32_t exit   = 1;
    while ( *(this->_axi_mmap_ptr+address) != value || exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    std::cout << "WAIT32[" << std::hex << address << "," << value << "," << cycles << "] = " << cnt << "," << exit << std::endl;
    #endif
    return exit;
}
