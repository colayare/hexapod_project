//-- IKINEMATICS IP REG MAP
#ifndef __IKINEMATICS_REG_MAP_H__
#define __IKINEMATICS_REG_MAP_H__

//-- Register MAP Index --
#define IK_REG_TRIG 0
#define IK_REG_CTRL 1
#define IK_REG_STAT 2
#define IK_REG_LEGC 3
#define IK_REG_IKIX 4
#define IK_REG_IKIY 5
#define IK_REG_IKIZ 6
#define IK_REG_IKQ1 4
#define IK_REG_IKQ2 5
#define IK_REG_IKQ3 6
#define IK_REG_OF01 7
#define IK_REG_OF02 8
#define IK_REG_OF03 9
#define IK_REG_OF04 10
#define IK_REG_OF05 11
#define IK_REG_OF06 12
#define IK_REG_OF07 13
#define IK_REG_OF08 14
#define IK_REG_OF09 15
#define IK_REG_OF10 16
#define IK_REG_OF11 17
#define IK_REG_OF12 18
#define IK_REG_OF13 19
#define IK_REG_OF14 20
#define IK_REG_OF15 21
#define IK_REG_OF16 22
#define IK_REG_OF17 23
#define IK_REG_OF18 24
#define IK_REG_KO01 25
#define IK_REG_KO02 26
#define IK_REG_KO03 27
#define IK_REG_KO04 28
#define IK_REG_KO05 29
#define IK_REG_KO06 30
#define IK_REG_KO07 31
#define IK_REG_KO08 32
#define IK_REG_KO09 33
#define IK_REG_KO10 34
#define IK_REG_KO11 35
#define IK_REG_KO12 36
#define IK_REG_KO13 37
#define IK_REG_KO14 38
#define IK_REG_KO15 39
#define IK_REG_KO16 40
#define IK_REG_KO17 41
#define IK_REG_KO18 42

//-- Registers Bit Map
//-- [REG_TRIG] Register Bit Map
#define IK_REG_CONTROL_CLEAR_REG    0x00000000
#define IK_REG_TRIG_WRITE_FIFO      0x00000001
#define IK_REG_TRIG_TRIGGER         0x00000002
#define IK_REG_TRIG_DIRECT_OUT      0x00000004

//-- [REG_LEGC] Register Bit Map 
#define IK_REG_LEGC_CTR_ONE_LEG     0x00000010
#define IK_REG_LEGC_CTR_MUX_LEG     0x00000000
#define IK_REG_LEGC_CTR_LEG_SEL_O   0
#define IK_REG_LEGC_CTR_LEG_SEL_M   0x00000007
#define IK_REG_LEGC_CTR_SET_LEG_T   0x00000008
#define IK_REG_LEGC_CTR_MODE_O      4
#define IK_REG_LEGC_CTR_MODE_M      0x00000030
#define IK_REG_LEGC_CTR_OUTPUT_O    6
#define IK_REG_LEGC_CTR_OUTPUT_M    0x000000C0
#define IK_REG_LEGC_F2F_READ_MUX_O  9
#define IK_REG_LEGC_F2F_READ_MUX_M  0x00000E00
#define IK_REG_LEGC_PWM_INVERT_O    12
#define IK_REG_LEGC_PWM_INVERT_M    0x3FFFF000

#endif //__IKINEMATICS_REG_MAP_H__
