/*H**********************************************************************
* FILENAME : ip_globals.c
*
* DESCRIPTION :
*       This file contains Global AXI IP functions
*
* PUBLIC FUNCTIONS :
*
* NOTES :
*
* AUTHOR :
*
* CHANGES :
*
*
*H*/
/***************************** Include Files *******************************/
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "ip_globals.h"

/************************** Function Definitions ***************************/
//-- IP Object Pointer Creator
// This Fucntion gets the Memory Map Pointer of an AXI IP and return the pointer
// value
//- INPUTS
// ip_context : IP context of the IP
//- RETURNS
// Pointer of the AXI IP
int32_t *create_ip_mmap(IP_CONTEXT_t ip_context) {
    int32_t _fdmem;
    int32_t *axi_mmap = NULL;
    const char memDevice[] = "/dev/mem";

    _fdmem = open( memDevice, O_RDWR | O_SYNC );
    if ( _fdmem < 0 ) {
        printf("Failed to open /dev/mem !\n");
        return 0;
    } else {
        printf("Open /dev/mem successfully !\n");
    }
    
    axi_mmap = (int32_t *)(mmap(0, (ip_context)->axi_map_size*4, PROT_READ|PROT_WRITE, MAP_SHARED, _fdmem, (ip_context)->axi_base_address));
    
    return axi_mmap;
}

//-- AXI IP memory map read data
//- INPUTS
// axi_mmap : Pointer to iKinematics AXI IP base address
// read_address : Relative address to read data (No consider base address)
// RETURNS 
// 32-bit read register
int32_t axi_read(int32_t *axi_mmap, int32_t read_address) {
    return *(axi_mmap+read_address);
}

//-- AXI IP memory map read data with mask
//- INPUTS
// axi_mmap : Pointer to iKinematics AXI IP base address
// read_address : Relative address to read data (No consider base address)
// mask : mask of read operation
// RETURNS 
// 32-bit read register
int32_t axi_read_mask(int32_t *axi_mmap, int32_t read_address, int32_t mask) {
    return *(axi_mmap+read_address) & mask;
}

//-- AXI IP memory map write data
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
// write_address : Relative address to write data (No consider base address)
void axi_write(int32_t *axi_mmap, int32_t write_address, int32_t value) {
    *(axi_mmap+write_address) = value;
}

//-- AXI IP memory map write data with mask
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
// write_address : Relative address to write data (No consider base address)
// mask : mask for write operation
void axi_write_mask(int32_t *axi_mmap, int32_t write_address, int32_t value, int32_t mask) {
    int32_t register_value;
    register_value = *(axi_mmap+write_address) & ~mask;
    *(axi_mmap+write_address) = register_value + (value & mask);
}

//-- AXI IP memory map bit set
//- Inputs
// axi_mmap : Pointer to iKinematics AXI IP base address
// address : Relative address to write data (No consider base address)
// mask : mask for write operation
void axi_bit_set(int32_t *axi_mmap, int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(axi_mmap+write_address) & ~bit_mask;
    *(axi_mmap+write_address) = register_value + bit_mask;
}

//-- AXI IP memory map bit clear
void axi_bit_clr(int32_t *axi_mmap, int32_t address, int32_t bit_mask) {
    int32_t register_value;
    register_value = *(axi_mmap+write_address) & ~bit_mask;
    *(axi_mmap+write_address) = register_value;
}