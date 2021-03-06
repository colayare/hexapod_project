#ifndef __AXI_IP_GLOBALS__
#define __AXI_IP_GLOBALS__
//==============================================================================
// Includes
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <linux/i2c-dev.h>

using String = std::string;

//==============================================================================
// AXI IP Global Class
class ip_context {
    //==== Members  ============================================================
    public:
        String   dev_name;                  //== Device Name (eg. /dev/mem, /dev/i2c-0)
        String   ip_name;                   //== AXI IP NAME
    
    private:
        volatile uint32_t *_axi_mmap_ptr;   //== AXI IP Memory Map Pointer
        int32_t  _ip_file;
        uint32_t _axi_mmap_size;            //== AXI IP Memory MAP Size in Bytes
        uint32_t _axi_base_address;         //== AXI IP Memory MAP Base Address
        uint32_t _axi_word_size;            //== AXI IP Memory Map Word Size
    
    //==== Method Prototypes ===================================================
    public:
        //== Constructor
        ip_context();
        ip_context(String ip_name);
        ip_context(String ip_name, char dev_name[], uint32_t axi_base_address, uint32_t axi_mmap_size);
        //== Destructor
        //~ip_context();
        //== Init AXI IP Memory Map
        uint32_t    init_axi_mmap_ptr(char dev_name[], uint32_t axi_base_address, uint32_t axi_mmap_size);
        //== Private Members Functions Getters
        volatile uint32_t *get_mmap_ptr();
        int32_t     ip_file();
        uint32_t    get_axi_mmap_size();
        uint32_t    get_axi_base_address();
        uint32_t    get_axi_word_size();
        
        //== AXI IP Operations 32-bit
        uint32_t    axi_read(uint32_t read_address);
        uint32_t    axi_read_mask(uint32_t read_address, uint32_t mask);
        void        axi_write(uint32_t write_address, uint32_t value);
        void        axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask);
        void        axi_bit_set(uint32_t address, uint32_t bit_mask);
        void        axi_bit_clr(uint32_t address, uint32_t bit_mask);
        void        axi_show_regs(uint32_t start_address, uint32_t end_address);
        uint32_t    axi_read_exp(uint32_t address, uint32_t expected_value);
        uint32_t    axi_wait(uint32_t address, uint32_t value, uint32_t cycles);
        uint32_t    axi_wait_mask(uint32_t address, uint32_t value, uint32_t mask, uint32_t cycles);

        //== AXI IP Operations 16-bit
        uint16_t    axi_read16(uint16_t read_address);
        uint16_t    axi_read_mask16(uint16_t read_address, uint16_t mask);
        void        axi_write16(uint16_t write_address, uint16_t value);
        void        axi_write_mask16(uint16_t write_address, uint16_t value, uint16_t mask);
        void        axi_bit_set16(uint16_t address, uint16_t bit_mask);
        void        axi_bit_clr16(uint16_t address, uint16_t bit_mask);
        void        axi_show_regs16(uint16_t start_address, uint16_t end_address);
        uint16_t    axi_read_exp16(uint16_t address, uint16_t expected_value);
        uint16_t    axi_wait16(uint16_t address, uint16_t value, uint16_t cycles);
        uint16_t    axi_wait_mask16(uint16_t address, uint16_t value, uint16_t mask, uint16_t cycles);
        
};

#endif //__AXI_IP_GLOBALS__
