//==== AXI IKINEMATICS IP Register Map ====
//-- Register Map Index --
`define REG_TRIG 00
`define REG_CTRL 01
`define REG_STAT 02
`define REG_LEGC 03
`define REG_IKIX 04
`define REG_IKIY 05
`define REG_IKIZ 06
`define REG_OF01 07
`define REG_OF02 08
`define REG_OF03 09
`define REG_OF04 10
`define REG_OF05 11
`define REG_OF06 12
`define REG_OF07 13
`define REG_OF08 14
`define REG_OF09 15
`define REG_OF10 16
`define REG_OF11 17
`define REG_OF12 18
`define REG_OF13 19
`define REG_OF14 20
`define REG_OF15 21
`define REG_OF16 22
`define REG_OF17 23
`define REG_OF18 24
`define REG_KO01 25
`define REG_KO02 26
`define REG_KO03 27
`define REG_KO04 28
`define REG_KO05 29
`define REG_KO06 30
`define REG_KO07 31
`define REG_KO08 32
`define REG_KO09 33
`define REG_KO10 34
`define REG_KO11 35
`define REG_KO12 36
`define REG_KO13 37
`define REG_KO14 38
`define REG_KO15 39
`define REG_KO16 40
`define REG_KO17 41
`define REG_KO18 42

//== AXI Slave Register Name
 `define  REG(reg_idx) slv_reg``reg_idx
 
//==== Bit Map ====
//-- Trigger Register
`define TRIG_FIFO 0
`define TRIG_CALC 1
`define TRIG_DRCT 2

//-- ControL Register
`define CTRL_TRIG     0
`define CTRL_TRIG_EN  1
 
//-- Leg Counter Register
`define LEGC_INSE_MSB 2
`define LEGC_INSE_LSB 0
`define LEGC_TRIG     3
`define LEGC_MSEL_MSB 5
`define LEGC_MSEL_LSB 4
`define LEGC_LPTR_MSB 8
`define LEGC_LPTR_LSB 6
`define LEGC_FXFC_MSB 11
`define LEGC_FXFC_LSB 9
`define LEGC_SINV_01  12
`define LEGC_SINV_02  13
`define LEGC_SINV_03  14
`define LEGC_SINV_04  15
`define LEGC_SINV_05  16
`define LEGC_SINV_06  17
`define LEGC_SINV_07  18
`define LEGC_SINV_08  19
`define LEGC_SINV_09  20
`define LEGC_SINV_10  21
`define LEGC_SINV_11  22
`define LEGC_SINV_12  23
`define LEGC_SINV_13  24
`define LEGC_SINV_14  25
`define LEGC_SINV_15  26
`define LEGC_SINV_16  27
`define LEGC_SINV_17  28
`define LEGC_SINV_18  29
