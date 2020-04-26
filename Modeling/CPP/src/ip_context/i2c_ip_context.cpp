#include "params/ip_reg_map.h"
#include "globals/axi_ip_globals.h"
#include "globals/global_defines.h"
#include "ip_context/i2c_ip_context.h"

uint8_t i2c_ip_context::reg_reset_check() {
    uint8_t result = 1;
    
    #ifdef __IP_LOG
    cout << "i2c_ip_context::reg_reset_check" << endl;
    #endif
    result &= this->axi_read_exp8(XIICPS_CR_L, XIICPS_CR_L_RST);
    result &= this->axi_read_exp8(XIICPS_CR_H, XIICPS_CR_H_RST);
    result &= this->axi_read_exp8(XIICPS_SR_L, XIICPS_SR_L_RST);
    result &= this->axi_read_exp8(XIICPS_SR_H, XIICPS_SR_H_RST);
    result &= this->axi_read_exp8(XIICPS_ADDR_L, XIICPS_ADDR_L_RST);
    result &= this->axi_read_exp8(XIICPS_ADDR_H, XIICPS_ADDR_H_RST);
    result &= this->axi_read_exp8(XIICPS_DATA_L, XIICPS_DATA_L_RST);
    result &= this->axi_read_exp8(XIICPS_DATA_H, XIICPS_DATA_H_RST);
    result &= this->axi_read_exp8(XIICPS_ISR_L, XIICPS_ISR_L_RST);
    result &= this->axi_read_exp8(XIICPS_ISR_H, XIICPS_ISR_H_RST);
    result &= this->axi_read_exp8(XIICPS_TRANS_SIZE, XIICPS_TRANS_SIZE_RST);
    result &= this->axi_read_exp8(XIICPS_SLV_PAUSE, XIICPS_SLV_PAUSE_RST);
    result &= this->axi_read_exp8(XIICPS_TIME_OUT, XIICPS_TIME_OUT_RST);
    result &= this->axi_read_exp8(XIICPS_IMR_L, XIICPS_IMR_L_RST);
    result &= this->axi_read_exp8(XIICPS_IMR_H, XIICPS_IMR_H_RST);
    result &= this->axi_read_exp8(XIICPS_IER_L, XIICPS_IER_L_RST);
    result &= this->axi_read_exp8(XIICPS_IER_H, XIICPS_IER_H_RST);
    result &= this->axi_read_exp8(XIICPS_IDR_L, XIICPS_IDR_L_RST);
    result &= this->axi_read_exp8(XIICPS_IDR_H, XIICPS_IDR_H_RST);
    
    return result;
}