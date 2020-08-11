/*
 * iKinematics_IP_params.h
 *
 *  Created on: 10 may. 2020
 *      Author: Roy
 */

#ifndef SRC_IKINEMATICS_IP_PARAMS_H_
#define SRC_IKINEMATICS_IP_PARAMS_H_

//-- Inverse Kinematics IP Register Map
#define REG_TRIG 0
#define REG_CTRL 1
#define REG_STAT 2
#define REG_LEGC 3
#define REG_IKIX 4
#define REG_IKIY 5
#define REG_IKIZ 6
#define REG_IKQ1 4
#define REG_IKQ2 5
#define REG_IKQ3 6
#define REG_OF01 7
#define REG_OF02 8
#define REG_OF03 9
#define REG_OF04 10
#define REG_OF05 11
#define REG_OF06 12
#define REG_OF07 13
#define REG_OF08 14
#define REG_OF09 15
#define REG_OF10 16
#define REG_OF11 17
#define REG_OF12 18
#define REG_OF13 19
#define REG_OF14 20
#define REG_OF15 21
#define REG_OF16 22
#define REG_OF17 23
#define REG_OF18 24
#define REG_KO01 25
#define REG_KO02 26
#define REG_KO03 27
#define REG_KO04 28
#define REG_KO05 29
#define REG_KO06 30
#define REG_KO07 31
#define REG_KO08 32
#define REG_KO09 33
#define REG_KO10 34
#define REG_KO11 35
#define REG_KO12 36
#define REG_KO13 37
#define REG_KO14 38
#define REG_KO15 39
#define REG_KO16 40
#define REG_KO17 41
#define REG_KO18 42

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
