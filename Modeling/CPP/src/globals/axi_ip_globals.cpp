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
ip_context::ip_context(char dev_name[]) {
    string dev(dev_name);
    this->dev_name = dev;
}

//== Init AXI IP Memory Map
uint32_t ip_context::init_axi_mmap_ptr(uint32_t axi_mmap_size, uint32_t axi_base_address, uint32_t axi_word_size) {
    int32_t _fdmem;
    volatile uint32_t *axi_mmap_ptr = NULL;
    size_t mmap_size = (size_t) (axi_mmap_size * axi_word_size);
    char memDevice[this->dev_name.size()];
    
    //== Get Device Name
    strcpy(memDevice, this->dev_name.c_str()); 
    
    //== Validate 
    if ( !( axi_mmap_size && axi_base_address ) ) {
        cout << "ERROR : AXI MMAP Size or Base Address is zero" << endl;
        return 0;
    }

    _fdmem = open( memDevice, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        cout << "Failed to open " << this->dev_name << " !\n" << endl;
        return 0;
    } else {
        cout << "Open " << this->dev_name << " successfully !\n" << endl;
    }
    
    axi_mmap_ptr = (uint32_t *)(mmap(0, mmap_size, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, axi_base_address));
    
    // Setting Class members values
    this->_axi_mmap_ptr      = axi_mmap_ptr;
    this->_ip_file           = _fdmem;
    this->_axi_mmap_size     = axi_mmap_size;
    this->_axi_base_address  = axi_base_address;
    this->_axi_word_size     = axi_word_size;
    
    return 1;
}
//== Get Memory Map Pointer
volatile uint32_t *ip_context::get_mmap_ptr() { return this->_axi_mmap_ptr; }
//== Get File
int32_t ip_context::ip_file() { return this->_ip_file; }

//==============================================================================
//== 32 Bit Operations
//==============================================================================
//== AXI IP Read
uint32_t ip_context::axi_read(uint32_t read_address) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address);
    cout << "R32[" << read_address << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
uint32_t ip_context::axi_read_mask(uint32_t read_address, uint32_t mask) {
    #ifdef __IP_LOG
    uint32_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    cout << "RM32[" << read_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write(uint32_t write_address, uint32_t value) {
    #ifdef __IP_LOG
    cout << "W32[" << write_address << "] = " << hex << value << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + (value & mask);
    cout << "WM32[" << write_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    cout << "BS32[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr(uint32_t address, uint32_t bit_mask) {
    uint32_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint32_t val_print = register_value + bit_mask;
    cout << "BC32[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs(uint32_t start_address, uint32_t end_address) {
    //TODO: Check counter i word size access
    for (uint32_t i=0;i<(end_address-start_address);i++) {
        cout << "REG32[" << i+start_address << "]\t= " << hex << *(this->_axi_mmap_ptr+start_address+i) << endl;
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
    cout << "RE32[" << hex << address << "," << expected_value << "] = " << *(this->_axi_mmap_ptr+address) << " = " << result << endl;
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
    cout << "WAIT32[" << hex << address << "," << value << "," << cycles << "] = " << cnt << "," << exit << endl;
    #endif
    return exit;
}
//== AXI IP Wait for Read Value with Mask
uint32_t ip_context::axi_wait_mask(uint32_t address, uint32_t value, uint32_t mask, uint32_t cycles) {
    uint32_t cnt    = 0;
    uint32_t exit   = 1;
    while ( (*(this->_axi_mmap_ptr+address) & mask) != value || exit ) {
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

//==============================================================================
//== 16 Bit Operations
//==============================================================================
//== AXI IP Read
uint16_t ip_context::axi_read16(uint16_t read_address) {
    #ifdef __IP_LOG
    uint16_t val_print = *(this->_axi_mmap_ptr+read_address);
    cout << "R16[" << read_address << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
uint16_t ip_context::axi_read_mask16(uint16_t read_address, uint16_t mask) {
    #ifdef __IP_LOG
    uint16_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    cout << "RM16[" << read_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write16(uint16_t write_address, uint16_t value) {
    #ifdef __IP_LOG
    cout << "W16[" << write_address << "] = " << hex << value << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask16(uint16_t write_address, uint16_t value, uint16_t mask) {
    uint16_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + (value & mask);
    cout << "WM16[" << write_address << "," << mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set16(uint16_t address, uint16_t bit_mask) {
    uint16_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + bit_mask;
    cout << "BS16[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr16(uint16_t address, uint16_t bit_mask) {
    uint16_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint16_t val_print = register_value + bit_mask;
    cout << "BC16[" << address << "," << bit_mask << "] = " << hex << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs16(uint16_t start_address, uint16_t end_address) {
    //TODO: Check counter i word size access
    for (uint16_t i=0;i<(end_address-start_address);i++) {
        cout << "REG16[" << i+start_address << "]\t= " << hex << *(this->_axi_mmap_ptr+start_address+i) << endl;
    }
}
//== AXI IP Read Expected Value
uint16_t ip_context::axi_read_exp16(uint16_t address, uint16_t expected_value) {
    uint16_t result;
    if ( *(this->_axi_mmap_ptr+address) == expected_value ) {
        result =  1;
    } else {
        
        result = 0;
    }
    #ifdef __IP_LOG
    cout << "RE16[" << hex << address << "," << expected_value << "] = " << *(this->_axi_mmap_ptr+address) << " = " << result << endl;
    #endif
    return result;
}
//== AXI IP Wait for a Read Value
uint16_t ip_context::axi_wait16(uint16_t address, uint16_t value, uint16_t cycles) {
    uint16_t cnt    = 0;
    uint16_t exit   = 1;
    while ( *(this->_axi_mmap_ptr+address) != value || exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    cout << "WAIT16[" << hex << address << "," << value << "," << cycles << "] = " << cnt << "," << exit << endl;
    #endif
    return exit;
}

//==============================================================================
//== 8 Bit Operations
//==============================================================================
//== AXI IP Read
uint8_t ip_context::axi_read8(uint8_t read_address) {
    uint32_t address;
    address = (uint32_t) read_address;
    #ifdef __IP_LOG
    uint8_t val_print = *(this->_axi_mmap_ptr+address);
    cout << "R8[" << hex << address << "] = " << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+address);
}
//== AXI IP Read with mask
uint8_t ip_context::axi_read_mask8(uint8_t read_address, uint8_t mask) {
    #ifdef __IP_LOG
    uint8_t val_print = *(this->_axi_mmap_ptr+read_address) & mask;
    cout << "RM8[" << hex << read_address << "," << mask << "] = " << val_print << endl;
    #endif
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
void ip_context::axi_write8(uint8_t write_address, uint8_t value) {
    #ifdef __IP_LOG
    cout << "W8[" << hex << write_address << "] = " << value << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = value;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask8(uint8_t write_address, uint8_t value, uint8_t mask) {
    uint8_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    #ifdef __IP_LOG
    uint8_t val_print = register_value + (value & mask);
    cout << "WM8[" << hex << write_address << "," << mask << "] = " << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
}
//== AXI IP Bit Set
void ip_context::axi_bit_set8(uint8_t address, uint8_t bit_mask) {
    uint8_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint8_t val_print = register_value + bit_mask;
    cout << "BS8[" << hex << address << "," << bit_mask << "] = " << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr8(uint8_t address, uint8_t bit_mask) {
    uint8_t register_value;
    register_value = *(this->_axi_mmap_ptr+address) & ~bit_mask;
    #ifdef __IP_LOG
    uint8_t val_print = register_value + bit_mask;
    cout << "BC8[" << hex << address << "," << bit_mask << "] = " << val_print << endl;
    #endif
    *(this->_axi_mmap_ptr+address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs8(uint8_t start_address, uint8_t end_address) {
    //TODO: Check counter i word size access
    for (uint8_t i=0;i<(end_address-start_address);i++) {
        cout << "REG8[" << hex << i+start_address << "]\t= " << *(this->_axi_mmap_ptr+start_address+i) << endl;
    }
}
//== AXI IP Read Expected Value
uint8_t ip_context::axi_read_exp8(uint8_t address, uint8_t expected_value) {
    uint8_t result;
    if ( *(this->_axi_mmap_ptr+address) == expected_value ) {
        result =  1;
    } else {
        
        result = 0;
    }
    #ifdef __IP_LOG
    cout << "RE8[" << hex << address << "," << expected_value << "] = " << *(this->_axi_mmap_ptr+address) << " = " << result << endl;
    #endif
    return result;
}
//== AXI IP Wait for a Read Value
uint8_t ip_context::axi_wait8(uint8_t address, uint8_t value, uint8_t cycles) {
    uint8_t cnt    = 0;
    uint8_t exit   = 1;
    while ( *(this->_axi_mmap_ptr+address) != value || exit ) {
        cnt++;
        if ( cnt == cycles ) {
            exit = 0;
        }
    }
    #ifdef __IP_LOG
    cout << "WAIT8[" << hex << address << "," << value << "," << cycles << "] = " << cnt << "," << exit << endl;
    #endif
    return exit;
}
