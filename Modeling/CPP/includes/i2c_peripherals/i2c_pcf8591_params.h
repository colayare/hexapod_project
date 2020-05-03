#ifndef __PCF9581_PARAMS_H__
#define __PCF9581_PARAMS_H__

#define MODE0   0x00    //Channel0 = AIN0;
                        //Channel1 = AIN1;
                        //Channel2 = AIN2;
                        //Channel3 = AIN3;
#define MODE1   0x10    //Channel0 = AIN3-AIN0;
                        //Channel1 = AIN3-AIN1
                        //Channel2 = AIN3-AIN2;
#define MODE2   0x20    //Channel0 = AIN0;
                        //Channel1 = AIN1;
                        //Channel2 = AIN3-AIN2;
#define MODE3   0x30    //Channel0 = AIN1-AIN0;
                        //Channel1 = AIN3-AIN2;
#define CHNL0   0       //Using Channel0
#define CHNL1   1       //Using Channel1
#define CHNL2   2       //Using Channel2
#define CHNL3   3       //Using Channel3

#define DAouputEn   0x40    //0x50 or 0x60 or 0x70 is also ok!

#define WD_DADR 0x90
#define RD_DADR 0x91

#define PCF8591_ADDR 0x48

#endif //__PCF9581_PARAMS_H__