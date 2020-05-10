/*
 * iKinematics_IP_params.h
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#ifndef SRC_IKINEMATICS_IP_PARAMS_H_
#define SRC_IKINEMATICS_IP_PARAMS_H_

//-- Inverse Kinematics IP Parameters
#define AXI_IK_BASEADDR 0x40000000
#define AXI_IK_HIGHADDR 0x4000FFFF
#define AXI_IK_PAGESIZE AXI_IK_HIGHADDR - AXI_IK_BASEADDR + 1
#define AXI_IK_RMAPSIZE 40
#define AXI_IK_WORDSIZE 4

//-- Inverse Kinematics IP Register Map
#define REG_TRIG 0
#define REG_LEGC 1
#define REG_IKIX 2
#define REG_IKIY 3
#define REG_IKIZ 4
#define REG_IKQ1 2
#define REG_IKQ2 3
#define REG_IKQ3 4
#define REG_OF01 5
#define REG_OF02 6
#define REG_OF03 7
#define REG_OF04 8
#define REG_OF05 9
#define REG_OF06 10
#define REG_OF07 11
#define REG_OF08 12
#define REG_OF09 13
#define REG_OF10 14
#define REG_OF11 15
#define REG_OF12 16
#define REG_OF13 17
#define REG_OF14 18
#define REG_OF15 19
#define REG_OF16 20
#define REG_OF17 21
#define REG_OF18 22
#define REG_KO01 23
#define REG_KO02 24
#define REG_KO03 25
#define REG_KO04 26
#define REG_KO05 27
#define REG_KO06 28
#define REG_KO07 29
#define REG_KO08 30
#define REG_KO09 31
#define REG_KO10 32
#define REG_KO11 33
#define REG_KO12 34
#define REG_KO13 35
#define REG_KO14 36
#define REG_KO15 37
#define REG_KO16 38
#define REG_KO17 39
#define REG_KO18 40

//- IP Control & Trigger Register
// Bit Definition
#define TRIG_FIFO 0
#define TRIG_CALC 1
#define TRIG_DRCT 2
// Common Values
#define TRIG_CLEAR_REG    0x00000000
#define TRIG_WRITE_FIFO   0x00000001
#define TRIG_IK_CALC      0x00000002
#define TRIG_WRITE_DRCT   0x00000004

//- Leg Counter Control Register
// Bit Definition
#define LEGC_INSE_MSB         2
#define LEGC_INSE_LSB         0
#define LEGC_TRIG             3
#define LEGC_MSEL_MSB         5
#define LEGC_MSEL_LSB         4
#define LEGC_LPTR_MSB         8
#define LEGC_LPTR_LSB         6
#define LEGC_FXFC_MSB         11
#define LEGC_FXFC_LSB         9
#define LEGC_SINV_01          12
#define LEGC_SINV_02          13
#define LEGC_SINV_03          14
#define LEGC_SINV_04          15
#define LEGC_SINV_05          16
#define LEGC_SINV_06          17
#define LEGC_SINV_07          18
#define LEGC_SINV_08          19
#define LEGC_SINV_09          20
#define LEGC_SINV_10          21
#define LEGC_SINV_11          22
#define LEGC_SINV_12          23
#define LEGC_SINV_13          24
#define LEGC_SINV_14          25
#define LEGC_SINV_15          26
#define LEGC_SINV_16          27
#define LEGC_SINV_17          28
#define LEGC_SINV_18          29
// Common Values
#define LEGC_CTR_ONE_LEG      0x00000010
#define LEGC_CTR_MUX_LEG      0x00000000
#define LEGC_CTR_LEG_SEL_M    0x00000007
#define LEGC_CTR_SET_LEG_T    0x00000008
#define LEGC_CTR_MODE_M       0x00000030
#define LEGC_CTR_OUTPUT_M     0x000000C0
#define LEGC_F2F_READ_MUX_M   0x00000E00
#define LEGC_PWM_INVERT_M     0x3FFFF000

#endif /* SRC_IKINEMATICS_IP_PARAMS_H_ */
