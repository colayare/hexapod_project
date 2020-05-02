#ifndef __AXI_IP_GLOBALS__
#define __AXI_IP_GLOBALS__
//==============================================================================
// Includes
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <linux/i2c-dev.h>

using namespace std;

//==============================================================================
// AXI IP Global Class
class ip_context {
    //==== Members  ============================================================
    public:
        uint32_t &axi_mmap_size( uint32_t );
        uint32_t &axi_base_address( uint32_t );
        uint32_t &axi_word_size( uint32_t );
        string   dev_name;                  //== Device Name (eg. /dev/mem, /dev/i2c-0)
        string   ip_name;                   //== AXI IP NAME
    
    private:
        volatile uint32_t *_axi_mmap_ptr;   //== AXI IP Memory Map Pointer
        int32_t  _ip_file;
        uint32_t _axi_mmap_size;            //== AXI IP Memory MAP Size in Bytes
        uint32_t _axi_base_address;         //== AXI IP Memory MAP Base Address
        uint32_t _axi_word_size;            //== AXI IP Memory Map Word Size
    
    //==== Method Prototypes ===================================================
    public:
        //== Constructor
        ip_context(char dev_name[]);
        //== Init AXI IP Memory Map
        uint32_t    init_axi_mmap_ptr(uint32_t axi_mmap_size, uint32_t axi_base_address, uint32_t axi_word_size);
        //== Get Private Members Functions
        volatile uint32_t *get_mmap_ptr();
        int32_t     ip_file();
        
        //== AXI IP Operations 32 Bit
        uint32_t    axi_read(uint32_t read_address);
        uint32_t    axi_read_mask(uint32_t read_address, uint32_t mask);
        void        axi_write(uint32_t write_address, uint32_t value);
        void        axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask);
        void        axi_bit_set(uint32_t address, uint32_t bit_mask);
        void        axi_bit_clr(uint32_t address, uint32_t bit_mask);
        void        axi_show_regs(uint32_t start_address, uint32_t end_address);
        uint32_t    axi_read_exp(uint32_t address, uint32_t expected_value);
        uint32_t    axi_wait(uint32_t address, uint32_t value, uint32_t cycles);
        
        //== AXI IP Operations 16 Bit
        uint16_t    axi_read16(uint16_t read_address);
        uint16_t    axi_read_mask16(uint16_t read_address, uint16_t mask);
        void        axi_write16(uint16_t write_address, uint16_t value);
        void        axi_write_mask16(uint16_t write_address, uint16_t value, uint16_t mask);
        void        axi_bit_set16(uint16_t address, uint16_t bit_mask);
        void        axi_bit_clr16(uint16_t address, uint16_t bit_mask);
        void        axi_show_regs16(uint16_t start_address, uint16_t end_address);
        uint16_t    axi_read_exp16(uint16_t address, uint16_t expected_value);
        uint16_t    axi_wait16(uint16_t address, uint16_t value, uint16_t cycles);
        
        //== AXI IP Operations 8 Bit
        uint8_t     axi_read8(uint8_t read_address);
        uint8_t     axi_read_mask8(uint8_t read_address, uint8_t mask);
        void        axi_write8(uint8_t write_address, uint8_t value);
        void        axi_write_mask8(uint8_t write_address, uint8_t value, uint8_t mask);
        void        axi_bit_set8(uint8_t address, uint8_t bit_mask);
        void        axi_bit_clr8(uint8_t address, uint8_t bit_mask);
        void        axi_show_regs8(uint8_t start_address, uint8_t end_address);
        uint8_t     axi_read_exp8(uint8_t address, uint8_t expected_value);
        uint8_t     axi_wait8(uint8_t address, uint8_t value, uint8_t cycles);
};

#endif //__AXI_IP_GLOBALS__