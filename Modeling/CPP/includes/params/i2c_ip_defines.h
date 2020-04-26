//-- I2C IP REG MAP
#ifndef __I2C_REG_MAP_H__
#define __I2C_REG_MAP_H__

//-- Register Map 8-bit: Relative Addresses
#define XIICPS_CR_L             0 // Control Register Low Address 8-bit mixed
#define XIICPS_CR_H             1 // Control Register High Address 8-bit mixed
#define XIICPS_SR_L             2 // Status register Low Address 8-bit ro
#define XIICPS_SR_H             3 // Status register High Address 8-bit ro
#define XIICPS_ADDR_L           4 // IIC Address register Low Address 8-bit mixed
#define XIICPS_ADDR_H           5 // IIC Address register High Address 8-bit mixed
#define XIICPS_DATA_L           6 // IIC data register Low Address 8-bit mixed
#define XIICPS_DATA_H           7 // IIC data register High Address 8-bit mixed
#define XIICPS_ISR_L            8 // IIC interrupt status register Low Address 8-bit mixed
#define XIICPS_ISR_H            9 // IIC interrupt status register High Address 8-bit mixed
#define XIICPS_TRANS_SIZE       10 // Transfer Size Register 8 rw
#define XIICPS_SLV_PAUSE        11 // Slave Monitor Pause Register 8 mixed
#define XIICPS_TIME_OUT         12 // Time out register 8 rw
#define XIICPS_IMR_L            13 // Interrupt mask register Low Address 8-bit ro
#define XIICPS_IMR_H            14 // Interrupt mask register High Address 8-bit ro
#define XIICPS_IER_L            15 // Interrupt Enable Register Low Address 8-bit mixed
#define XIICPS_IER_H            16 // Interrupt Enable Register High Address 8-bit mixed
#define XIICPS_IDR_L            17 // Interrupt Disable Register Low Address 8-bit mixed
#define XIICPS_IDR_H            18 // Interrupt Disable Register High Address 8-bit mixed

//-- Reset Values
#define XIICPS_CR_L_RST             0x00 // Control Register 8-bit mixed
#define XIICPS_CR_H_RST             0x00 // Control Register 8-bit mixed
#define XIICPS_SR_L_RST             0x00 // Status register 8-bit ro
#define XIICPS_SR_H_RST             0x00 // Status register 8-bit ro
#define XIICPS_ADDR_L_RST           0x00 // IIC Address register 8-bit mixed
#define XIICPS_ADDR_H_RST           0x00 // IIC Address register 8-bit mixed
#define XIICPS_DATA_L_RST           0x00 // IIC data register 8-bit mixed
#define XIICPS_DATA_H_RST           0x00 // IIC data register 8-bit mixed
#define XIICPS_ISR_L_RST            0x00 // IIC interrupt status register 8-bit mixed
#define XIICPS_ISR_H_RST            0x00 // IIC interrupt status register 8-bit mixed
#define XIICPS_TRANS_SIZE_RST       0x00 // Transfer Size Register 8 rw
#define XIICPS_SLV_PAUSE_RST        0x00 // Slave Monitor Pause Register 8 mixed
#define XIICPS_TIME_OUT_RST         0x1F // Time out register 8 rw
#define XIICPS_IMR_L_RST            0xFF // Interrupt mask register 8-bit ro
#define XIICPS_IMR_H_RST            0x02 // Interrupt mask register 8-bit ro
#define XIICPS_IER_L_RST            0x00 // Interrupt Enable Register 8-bit mixed
#define XIICPS_IER_H_RST            0x00 // Interrupt Enable Register 8-bit mixed
#define XIICPS_IDR_L_RST            0x00 // Interrupt Disable Register 8-bit mixed
#define XIICPS_IDR_H_RST            0x00 // Interrupt Disable Register 8-bit mixed

#endif // __I2C_REG_MAP_H__