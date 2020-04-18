#ifndef __AXI_IP_GLOBALS__
#define __AXI_IP_GLOBALS__
//==============================================================================
// Includes
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "data_types.h"

//==============================================================================
// AXI IP Global Class
class ip_context {
    //==== Members  ============================================================
    public:
        int32_t &*axi_mmap_ptr( int32_t );
        int32_t &axi_mmap_size( int32_t );
        int32_t &axi_base_address( int32_t );
        int32_t &axi_word_size( int32_t );
        
    protected:
        string  ip_name;            //== AXI IP NAME
    
    private:
        int32_t *_axi_mmap_ptr;      //== AXI IP Memory Map Pointer
        int32_t _axi_mmap_size;      //== AXI IP Memory MAP Size in Bytes
        int32_t _axi_base_address;   //== AXI IP Memory MAP Base Address
        int32_t _axi_word_size;      //== AXI IP Memory Map Word Size
    
    //==== Method Prototypes ===================================================
    //== Init AXI IP Memory Map
    extern int32_t init_axi_mmap_ptr(int32_t axi_mmap_size, int32_t axi_base_address, int32_t axi_word_size);
    //== IP Context Class Members Access
    extern int32_t get_base_address();
    extern int32_t get_mmap_size();
    extern int32_t get_word_size();
    extern int32_t *get_mmap_ptr();
    //== AXI IP Operations
    extern int32_t axi_read(int32_t read_address);
    extern int32_t axi_read_mask(int32_t read_address, int32_t mask);
    extern int32_t axi_write(int32_t write_address, int32_t value);
    extern int32_t axi_write_mask(int32_t write_address, int32_t value, int32_t mask);
    extern int32_t axi_bit_set(int32_t address, int32_t bit_mask);
    extern int32_t axi_bit_clr(int32_t address, int32_t bit_mask);
    extern int32_t axi_show_regs(int32_t start_address, int32_t end_address);
}

//==== Method Defines ==========================================================
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

//== AXI IP Read
int32_t ip_context::axi_read(int32_t read_address) {
    return *(this->_axi_mmap_ptr+read_address);
}
//== AXI IP Read with mask
int32_t ip_context::axi_read_mask(int32_t read_address, int32_t mask) {
    return *(this->_axi_mmap_ptr+read_address) & mask;
}
//== AXI IP Write
int32_t ip_context::axi_write(int32_t write_address, int32_t value) {
    *(this->_axi_mmap_ptr+write_address) = value;
    return 1;
}
//== AXI IP Write with Mask
void ip_context::axi_write_mask(int32_t write_address, int32_t value, int32_t mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~mask;
    *(this->_axi_mmap_ptr+write_address) = register_value + (value & mask);
    return 1;
}
//== AXI IP Bit Set
void ip_context::axi_bit_set(int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~bit_mask;
    *(this->_axi_mmap_ptr+write_address) = register_value + bit_mask;
}
//== AXI IP Bit Clear
void ip_context::axi_bit_clr(int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(this->_axi_mmap_ptr+write_address) & ~bit_mask;
    *(this->_axi_mmap_ptr+write_address) = register_value;
}
//== AXI IP Show Registers
void ip_context::axi_show_regs(int32_t start_address, int32_t end_address) {
    //TODO: Check counter i word size access
    for (int32_t i=0;i<(end_address-start_address);i++) {
        cout << "REG[" << i << "]\t= " << *(this->_axi_mmap_ptr+i) << endl;
    }
}

#endif //__AXI_IP_GLOBALS__