#ifndef GLOBALS_H
#define GLOBALS_H
/**** Data Types **************************************************************/
typedef long int int32_t;
typedef int int16_t;

typedef struct IP_CONTEXT_t {
    //-- IP Memory Map Description
    int32_t axi_map_size;
    int32_t axi_base_address;
    //-- IP Memory Map Pointer
    int32_t axi_ip_ptr;
} IP_CONTEXT_t;

/**** Function Prototypes *****************************************************/
//-- IP Object Pointer Creator
int32_t *create_ip_mmap(IP_CONTEXT_t ip_context);
extern int32_t axi_read(int32_t *axi_mmap, int32_t read_address);
extern int32_t axi_read_mask(int32_t *axi_mmap, int32_t read_address, int32_t mask);
extern void axi_write(int32_t *axi_mmap, int32_t write_address, int32_t value);
extern void axi_write_mask(int32_t *axi_mmap, int32_t write_address, int32_t value, int32_t mask);
extern void axi_bit_set(int32_t *axi_mmap, int32_t address, int32_t bit_mask);
extern void axi_bit_clr(int32_t *axi_mmap, int32_t address, int32_t bit_mask);

#endif // GLOBALS_H