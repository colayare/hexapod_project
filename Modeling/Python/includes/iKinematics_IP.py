# -*- coding: utf-8 -*-
###############################################################################
#### Inverse Kinematics AXI IP Register MAP & Defines class
###############################################################################
class iKinematics_IP(object):
    ###########################################################################
    #### Memory MAP
    ###########################################################################
    REG_TRIG=0
    REG_CTRL=1
    REG_STAT=2
    REG_LEGC=3
    REG_IKIX=4
    REG_IKIY=5
    REG_IKIZ=6
    REG_OF01=7
    REG_OF02=8
    REG_OF03=9
    REG_OF04=10
    REG_OF05=11
    REG_OF06=12
    REG_OF07=13
    REG_OF08=14
    REG_OF09=15
    REG_OF10=16
    REG_OF11=17
    REG_OF12=18
    REG_OF13=19
    REG_OF14=20
    REG_OF15=21
    REG_OF16=22
    REG_OF17=23
    REG_OF18=24
    REG_KO01=25
    REG_KO02=26
    REG_KO03=27
    REG_KO04=28
    REG_KO05=29
    REG_KO06=30
    REG_KO07=31
    REG_KO08=32
    REG_KO09=33
    REG_KO10=34
    REG_KO11=35
    REG_KO12=36
    REG_KO13=37
    REG_KO14=38
    REG_KO15=39
    REG_KO16=40
    REG_KO17=41
    REG_KO18=42
    ###########################################################################
    #### Defines 
    ###########################################################################
    # [REG_TRIG] Trigger Register
    REG_TRIG_CLEAR_REG    = 0x00000000
    REG_TRIG_WRITE_FIFO   = 0x00000001
    REG_TRIG_TRIGGER      = 0x00000002
    REG_TRIG_DIRECT_OUT   = 0x00000004

    # [REG_LEGC] Leg Counter Control Register
    REG_LEGC_CTR_ONE_LEG      = 0x00000010
    REG_LEGC_CTR_MUX_LEG      = 0x00000000
    REG_LEGC_CTR_LEG_SEL_O    = 0
    REG_LEGC_CTR_LEG_SEL_M    = 0x00000007
    REG_LEGC_CTR_SET_LEG_T    = 0x00000008
    REG_LEGC_CTR_MODE_O       = 4
    REG_LEGC_CTR_MODE_M       = 0x00000030
    REG_LEGC_CTR_OUTPUT_O     = 6
    REG_LEGC_CTR_OUTPUT_M     = 0x000000C0
    REG_LEGC_F2F_READ_MUX_O   = 9
    REG_LEGC_F2F_READ_MUX_M   = 0x00000E00
    REG_LEGC_PWM_INVERT_O     = 12
    REG_LEGC_PWM_INVERT_M     = 0x3FFFF000
            
