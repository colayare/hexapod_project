#ifndef __AXI_IP_GLOBALS__
#define __AXI_IP_GLOBALS__
//==============================================================================
// Includes
#include<iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string>

using namespace std;

//==============================================================================
// AXI IP Global Class
class ip_context {
    //==== Members  ============================================================
    public:
        //int32_t &*axi_mmap_ptr( int32_t );
        int32_t &axi_mmap_size( int32_t );
        int32_t &axi_base_address( int32_t );
        int32_t &axi_word_size( int32_t );
        
    // protected:
        // string  ip_name;            //== AXI IP NAME
    
    private:
        int32_t *_axi_mmap_ptr;      //== AXI IP Memory Map Pointer
        int32_t _axi_mmap_size;      //== AXI IP Memory MAP Size in Bytes
        int32_t _axi_base_address;   //== AXI IP Memory MAP Base Address
        int32_t _axi_word_size;      //== AXI IP Memory Map Word Size
    
    //==== Method Prototypes ===================================================
    public:
        //== Init AXI IP Memory Map
        int32_t init_axi_mmap_ptr(int32_t axi_mmap_size, int32_t axi_base_address, int32_t axi_word_size);
        //== Get Private Members Functions
        int32_t *get_mmap_ptr();
        //== AXI IP Operations
        int32_t axi_read(int32_t read_address);
        int32_t axi_read_mask(int32_t read_address, int32_t mask);
        void    axi_write(int32_t write_address, int32_t value);
        void    axi_write_mask(int32_t write_address, int32_t value, int32_t mask);
        void    axi_bit_set(int32_t address, int32_t bit_mask);
        void    axi_bit_clr(int32_t address, int32_t bit_mask);
        void    axi_show_regs(int32_t start_address, int32_t end_address);
};

#endif //__AXI_IP_GLOBALS__