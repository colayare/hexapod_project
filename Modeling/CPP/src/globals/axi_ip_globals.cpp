#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <string.h>
#include "globals/axi_ip_globals.h"
#include "globals/global_defines.h"

//==============================================================================
//== Class Methods
//==============================================================================
//== Constructor
ip_context::ip_context(): ip_name("Unkown_IP") {

}

ip_context::ip_context(String ip_name) {
    this->ip_name = ip_name;
    #ifdef __IP_LOG
    std::cout.fill('0');
    #endif
}

ip_context::ip_context(String ip_name, char dev_name[], uint32_t axi_base_address, uint32_t axi_mmap_size) {
    this->ip_name = ip_name;
    this->init_axi_mmap_ptr(dev_name, axi_base_address, axi_mmap_size);
}

//ip_context::~ip_context() {}
//== Private Members Function Getters
//== Get Memory Map Pointer
volatile uint32_t *ip_context::get_mmap_ptr() { return this->_axi_mmap_ptr; }
//== Get File
int32_t     ip_context::ip_file() { return this->_ip_file; }
uint32_t    ip_context::get_axi_mmap_size() { return this->_axi_mmap_size; }
uint32_t    ip_context::get_axi_base_address() { return this->_axi_base_address; }
uint32_t    ip_context::get_axi_word_size() { return this->_axi_word_size; }

//== Init AXI IP Memory Map
uint32_t    ip_context::init_axi_mmap_ptr(char dev_name[], uint32_t axi_base_address, uint32_t axi_mmap_size) {
    // File
    int32_t _fdmem;
    // Memory Map Pointer
    volatile uint32_t *axi_mmap_ptr = NULL;
    
    // Validate 
    if ( !( axi_mmap_size | axi_base_address ) ) {
        std::cout << "ERROR : AXI MMAP Size or Base Address is zero" << std::endl;
        return 0;
    }

    // Open File
    //_fdmem = open( memDevice, O_RDWR | O_SYNC );
    _fdmem = open( dev_name, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        std::cout << "Failed to open " << this->dev_name << " !\n" << std::endl;
        return 0;
    } else {
        std::cout << "Open " << this->dev_name << " successfully !\n" << std::endl;
    }
    
    //axi_mmap_ptr = (uint32_t *)(mmap(NULL, axi_mmap_size, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, mapping_n * axi_mmap_size));
    axi_mmap_ptr = (uint32_t *)(mmap(NULL, axi_mmap_size, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem,  axi_base_address));
    
    // Setting Class members values
    this->dev_name           = dev_name;
    this->_axi_mmap_ptr      = axi_mmap_ptr;
    this->_ip_file           = _fdmem;
    this->_axi_mmap_size     = axi_mmap_size;
    this->_axi_base_address  = axi_base_address;
    
    return 1;
}

//==============================================================================
//== 32 Bit Operations
//==============================================================================
//== AXI IP Read
uint32_t ip_context::axi_read(uint32_t read_address) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address);
    std::cout << std::hex << "R32[" << std::setw(8) << read_address << "] = " << std::setw(8) << val_print << std::endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
uint32_t ip_context::axi_read_mask(uint32_t read_address, uint32_t mask) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    std::cout << std::hex << "RM32[" << std::setw(8) << read_address << "," << std::setw(8) << mask << "] = " << std::setw(8) << val_print << std::endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write(uint32_t write_address, uint32_t value) {
    #ifdef __IP_LOG
    std::cout << std::hex << "W32[" << std::setw(8) << write_address << "] = " << std::setw(8) << value << std::endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + (value & mask);
    std::cout << std::hex << "WM32[" << std::setw(8) << write_address << "," << std::setw(8) << value << "," << std::setw(8) << mask << "] = " << setw(8) << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    std::cout << std::hex << "BS32[" << std::setw(8) << address << "," << std::setw(8) << bit_mask << "] = " << std::setw(8) << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    std::cout << std::hex << "BC32[" << std::setw(8) << address << "," << std::setw(8) << bit_mask << "] = " << std::setw(8) << val_print << std::endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs(uint32_t start_address, uint32_t end_address) {
    //TODO: Check counter i word size access
    for (uint32_t i=0;i<=(end_address-start_address);i++) {
        std::cout << std::hex << "REG32[" << std::setw(8) << i+start_address << "]\t= " << std::setw(8) << *(this->_axi_mmap_ptr+start_address+i) << std::endl;
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
    std::cout << "RE32[" << std::hex << std::setw(8) << address << "," << std::setw(8) << expected_value << "] = " << std::setw(8) << *(this->_axi_mmap_ptr+address) << " = " << result << std::endl;
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
    std::cout << std::hex << "WAIT32[" << std::setw(8) << address << "," << std::setw(8) << value << "," << std::dec << cycles << "] = " << cnt << "," << exit << std::endl;
    #endif
    return exit;
}
//== AXI IP Wait for Read Value with Mask
uint32_t ip_context::axi_wait_mask(uint32_t address, uint32_t value, uint32_t mask, uint32_t cycles) {
    uint32_t cnt    = 0;
    uint32_t exit   = 1;
    while ( (*(this->_axi_mmap_ptr+address) & mask) != value && exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    std::cout << std::hex << "WAIT32[" << std::setw(8) << address << "," << std::setw(8) << value << "," << std::dec << cycles << "] = " << cnt << "," << exit << std::std::endl;
    #endif
    return exit;
}

//==============================================================================
//== 16 Bit Operations
//==============================================================================
//== AXI IP Read
uint16_t ip_context::axi_read16(uint16_t read_address) {
    #ifdef __IP_LOG
    uint16_t val_print = *((uint16_t *)this->_axi_mmap_ptr+read_address);
    std::cout << std::hex << "R16[" << std::setw(4) << read_address << "] = " << std::setw(4) << val_print << std::endl;
    #endif
    return *((uint16_t *)this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
uint16_t ip_context::axi_read_mask16(uint16_t read_address, uint16_t mask) {
    #ifdef __IP_LOG
    uint16_t val_print = *((uint16_t *)this->_axi_mmap_ptr+read_address) & mask;
    std::cout << std::hex << "RM16[" << std::setw(4) << read_address << "," << std::setw(4) << mask << "] = " << std::setw(4) << val_print << std::endl;
    #endif
    return *((uint16_t *)this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask16(uint16_t write_address, uint16_t value, uint16_t mask) {
    uint16_t register_value;
    register_value = *((uint16_t *)this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + (value & mask);
    std::cout << std::hex << "WM16[" << std::setw(4) << write_address << "," << std::setw(4) << value << "," << std::setw(4) << mask << "] = " << setw(4) << val_print << std::endl;
    #endif
    *((uint16_t *)this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set16(uint16_t address, uint16_t bit_mask) {
    uint16_t register_value;
    register_value = *((uint16_t *)this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + bit_mask;
    std::cout << std::hex << "BS16[" << std::setw(4) << address << "," << std::setw(4) << bit_mask << "] = " << std::setw(4) << val_print << std::endl;
    #endif
    *((uint16_t *)this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr16(uint16_t address, uint16_t bit_mask) {
    uint16_t register_value;
    register_value = *((uint16_t *)this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + bit_mask;
    std::cout << std::hex << "BC16[" << std::setw(4) << address << "," << std::setw(4) << bit_mask << "] = " << std::setw(4) << val_print << std::endl;
    #endif
    *((uint16_t *)this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs16(uint16_t start_address, uint16_t end_address) {
    //TODO: Check counter i word size access
    for (uint16_t i=0;i<=(end_address-start_address);i++) {
        std::cout << std::hex << "REG16[" << std::setw(4) << i+start_address << "]\t= " << std::setw(4) << *((uint16_t *)this->_axi_mmap_ptr+start_address+i) << std::endl;
    }
}
//== AXI IP Read Expected Value
uint16_t ip_context::axi_read_exp16(uint16_t address, uint16_t expected_value) {
    uint16_t result;
    if ( *((uint16_t *)this->_axi_mmap_ptr+address) == expected_value ) {
        result =  1;
    } else {
        
        result = 0;
    }
    #ifdef __IP_LOG
    std::cout << "RE16[" << std::hex << std::setw(4) << address << "," << std::setw(4) << expected_value << "] = " << std::setw(4) << *((uint16_t *)this->_axi_mmap_ptr+address) << " = " << result << std::endl;
    #endif
    return result;
}
//== AXI IP Wait for a Read Value
uint16_t ip_context::axi_wait16(uint16_t address, uint16_t value, uint16_t cycles) {
    uint16_t cnt    = 0;
    uint16_t exit   = 1;
    while ( *((uint16_t *)this->_axi_mmap_ptr+address) != value || exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    std::cout << std::hex << "WAIT16[" << std::setw(4) << address << "," << std::setw(4) << value << "," << std::dec << cycles << "] = " << cnt << "," << exit << std::endl;
    #endif
    return exit;
}
//== AXI IP Wait for Read Value with Mask
uint16_t ip_context::axi_wait_mask16(uint16_t address, uint16_t value, uint16_t mask, uint16_t cycles) {
    uint16_t cnt    = 0;
    uint16_t exit   = 1;
    while ( (*((uint16_t *)this->_axi_mmap_ptr+address) & mask) != value && exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    std::cout << std::hex << "WAIT16[" << std::setw(4) << address << "," << std::setw(4) << value << "," << std::dec << cycles << "] = " << cnt << "," << exit << std::std::endl;
    #endif
    return exit;
}
