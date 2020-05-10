/*
 * ip_context.h
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#ifndef SRC_IP_CONTEXT_H_
#define SRC_IP_CONTEXT_H_

#include <iostream>

class ip_context {

  private:
    volatile uint32_t *_axi_mmap_ptr;
    uint32_t  _axi_mmap_size;
    uint32_t  _axi_base_address;
    uint32_t  _axi_word_size;

  public:
    volatile uint32_t *axi_mmap_ptr() { return this->_axi_mmap_ptr; }
    uint32_t  axi_mmap_size() { return this->_axi_mmap_size; }
    uint32_t  axi_base_address() { return this->_axi_base_address; }
    uint32_t  axi_word_size() { return this->_axi_word_size; }

    uint32_t  axi_init(uint32_t axi_base_address, uint32_t axi_mmap_size, uint32_t axi_word_size);
    uint32_t  axi_read(uint32_t read_address);
    uint32_t  axi_read_mask(uint32_t read_address, uint32_t mask);
    void      axi_write(uint32_t write_address, uint32_t value);
    void      axi_write_mask(uint32_t write_address, uint32_t value, uint32_t mask);
    void      axi_bit_set(uint32_t address, uint32_t bit_mask);
    void      axi_bit_clr(uint32_t address, uint32_t bit_mask);
    void      axi_show_regs(uint32_t start_address, uint32_t end_address);
    uint32_t  axi_read_exp(uint32_t address, uint32_t expected_value);
    uint32_t  axi_wait(uint32_t address, uint32_t value, uint32_t cycles);
};


#endif /* SRC_IP_CONTEXT_H_ */
