# Inverse Kinematics IP Register Map

#### Legend
__Register Modes__\
[T]   : Trigger\
[R]   : Read\
[W]   : Write\
__Numeric values__\
N     : Decimal\
0xN   : Hexadecimal Format\
0bN   : Binary Format\

## REGISTER MAP
|ADDR|REG NAME|Comment|
|------|------|------|
|0x00||Trigger Register|
|0x01||Leg Counter Register|
|0x02||Inverse Kinematics Input X|
|0x03||Inverse Kinematics Input Y|
|0x04||Inverse Kinematics Input Z|
|0x05||Digital Offset Q1|
|0x06||Digital Offset Q2|
|0x07||Digital Offset Q3|
|0x08||Digital Offset Q4|
|0x09||Digital Offset Q5|
|0x0A||Digital Offset Q6|
|0x0B||Digital Offset Q7|
|0x0C||Digital Offset Q8|
|0x0D||Digital Offset Q9|
|0x0E||Digital Offset Q10|
|0x0F||Digital Offset Q11|
|0x10||Digital Offset Q12|
|0x11||Digital Offset Q13|
|0x12||Digital Offset Q14|
|0x13||Digital Offset Q15|
|0x14||Digital Offset Q16|
|0x15||Digital Offset Q17|
|0x16||Digital Offset Q18|
|0x17||Leg 1 - Q1|
|0x18||Leg 1 - Q2|
|0x19||Leg 1 - Q3|
|0x1A||Leg 2 - Q1|
|0x1B||Leg 2 - Q2|
|0x1C||Leg 2 - Q3|
|0x1D||Leg 3 - Q1|
|0x1E||Leg 3 - Q2|
|0x1F||Leg 3 - Q3|
|0x20||Leg 4 - Q1|
|0x21||Leg 4 - Q2|
|0x22||Leg 4 - Q3|
|0x23||Leg 5 - Q1|
|0x24||Leg 5 - Q2|
|0x25||Leg 5 - Q3|
|0x26||Leg 6 - Q1|
|0x27||Leg 6 - Q2|
|0x28||Leg 6 - Q3|

----
# REGISTER SPECIFICATION 32-bit Register Size

## Trigger Register
__Address__ = 0x00\
__Word__    = 32-bit

__Register Description__\
This Register contains functionability triggers

__Bit Description__
|Bit|Type|Description|
|------|------|------|
|0|T|Trigger Fifo Write|
|1|T|Trigger Inverse Kinematics Calculation|
|2|T|Write angular value direct to Output Trigger|
|31:3|-|Unused|

## Leg Counter Register
__Address__ = 0x01
__Word__    = 32-bit

__Register Description__
    This Register handles Leg Counter Module

__Bit Description__
|Bit|Type|Description|
|------|------|------|
|2:0|W|Leg input selector|
|3|T|Set leg input|
|5:4|W|<p>Select counter mode<br>00b : Incremental ctr mode<br>01b : Static leg<br>10b - 11b : Forbiden</p>|
|8:6|RW|Current leg counter pointer|
|11:9|RW|<p>Output Fixed Point to Floating Point converter selector<br>0 : Inverse Kinematics Result<br>1 : Leg 1<br>2 : Leg 2<br>3 : Leg 3<br>4 : Leg 4<br>5 : Leg 5<br>6 : Leg 6<br>7 : Forbiden</p>|
|12|RW|<p>Leg 1 Servo Controler PWM Inversion<br>0 : No invert PWM<br>1 : Invert PWM</p>|
|13|RW|Leg 2 Servo Controler PWM Inversion|
|14|RW|Leg 3 Servo Controler PWM Inversion|
|15|RW|Leg 4 Servo Controler PWM Inversion|
|16|RW|Leg 5 Servo Controler PWM Inversion|
|17|RW|Leg 6 Servo Controler PWM Inversion|
|18|RW|Leg 7 Servo Controler PWM Inversion|
|19|RW|Leg 8 Servo Controler PWM Inversion|
|20|RW|Leg 9 Servo Controler PWM Inversion|
|21|RW|Leg 10 Servo Controler PWM Inversion|
|22|RW|Leg 11 Servo Controler PWM Inversion|
|23|RW|Leg 12 Servo Controler PWM Inversion|
|24|RW|Leg 13 Servo Controler PWM Inversion|
|25|RW|Leg 14 Servo Controler PWM Inversion|
|26|RW|Leg 15 Servo Controler PWM Inversion|
|27|RW|Leg 16 Servo Controler PWM Inversion|
|28|RW|Leg 17 Servo Controler PWM Inversion|
|29|RW|Leg 18 Servo Controler PWM Inversion|
|31:30|-|Unused|

## Inverse Kinematics Input X 
__Address__ = 0x02\
__Word__    = 32-bit\
\
__Register Description__\
   This registers works as Inverse Kinematics input when write and Inverse Kinematics output calculation when read
\
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Floating-Point Inverse Kinematics IP Parameter<br>[READ]  = IP iKinematics Output Parameter Q1<br>[WRITE] = IP iKinematics Input Parameter X|

## Inverse Kinematics Input Y 
__Address__ = 0x03\
__Word__    = 32-bit\
\
__Register Description__\
   This registers works as Inverse Kinematics input when write and Inverse Kinematics output calculation when read\
\
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Floating-Point Inverse Kinematics IP Parameter<br>[READ]  = IP iKinematics Output Parameter Q2<br>[WRITE] = IP iKinematics Input Parameter Y|


## Inverse Kinematics Input Z 
__Address__ = 0x04
__Word__    = 32-bit
\
__Register Description__
   This registers works as Inverse Kinematics input when write and Inverse Kinematics output calculation when read
\ 
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Floating-Point Inverse Kinematics IP Parameter<br>[READ]  = IP iKinematics Output Parameter Q3<br>[WRITE] = IP iKinematics Input Parameter Z|

## Digital Offset Q1
__Address__ = 0x05\
__Word__    = 32-bit\
\
__Register Description__
This Registers contains the angular value which is added to the Inverse Kinematics Leg1 - Q1 result for Calculate PWM Servo Value
\
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Leg 1 - Q1 Floating-Point Digital Offset|

## Digital Offset Q2
__Address__ = 0x06
__Word__    = 32-bit
\
__Register Description__
This Registers contains the angular value which is added to the Inverse Kinematics Leg 1 - Q2 result for Calculate PWM Servo Value
\
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Leg 1 - Q2 Floating-Point Digital Offset|

## Digital Offset Q3
__Address__ = 0x07\
__Word__    = 32-bit\
\
__Register Description__
This Registers contains the angular value which is added to the Inverse Kinematics Leg 1 - Q3 result for Calculate PWM Servo Value
\
__Bit Description__
|Bit|Type|Description|
|------|------|------|
|31:0|RW|Leg 1 - Q3 Floating-Point Digital Offset|

## Digital Offset Q4
__Address__ = 0x08
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg2 - Q1 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 2 - Q1 Floating-Point Digital Offset

## Digital Offset Q5
__Address__ = 0x09
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 2 - Q2 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 2 - Q2 Floating-Point Digital Offset

## Digital Offset Q6
__Address__ = 0x0A
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 2 - Q3 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 2 - Q3 Floating-Point Digital Offset

## Digital Offset Q7
__Address__ = 0x0B
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg3 - Q1 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 3 - Q1 Floating-Point Digital Offset

## Digital Offset Q8
__Address__ = 0x0C
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 3 - Q2 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 3 - Q2 Floating-Point Digital Offset

## Digital Offset Q9
__Address__ = 0x0D
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 3 - Q3 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 3 - Q3 Floating-Point Digital Offset

## Digital Offset Q10
__Address__ = 0x0E
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 4 - Q1 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 4 - Q1 Floating-Point Digital Offset

## Digital Offset Q11
__Address__ = 0x0F
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 4 - Q2 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 4 - Q2 Floating-Point Digital Offset

## Digital Offset Q12
__Address__ = 0x10
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 4 - Q3 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 4 - Q3 Floating-Point Digital Offset

## Digital Offset Q13
__Address__ = 0x11
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 5 - Q1 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 5 - Q1 Floating-Point Digital Offset

## Digital Offset Q14
__Address__ = 0x12
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 5 - Q2 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 5 - Q2 Floating-Point Digital Offset

## Digital Offset Q15
__Address__ = 0x13
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 5 - Q3 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 5 - Q3 Floating-Point Digital Offset

## Digital Offset Q16
__Address__ = 0x14
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 6 - Q1 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 6 - Q1 Floating-Point Digital Offset

## Digital Offset Q17
__Address__ = 0x15
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 6 - Q2 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 6 - Q2 Floating-Point Digital Offset

## Digital Offset Q18
__Address__ = 0x16
__Word__    = 32-bit

__Register Description__
    This Registers contains the angular value which is added to the Inverse Kinematics Leg 6 - Q3 result for Calculate PWM Servo Value

__Bit Description__
    31:0 [RW] = Leg 6 - Q3 Floating-Point Digital Offset

## Leg 1 - Q1
__Address__ = 0x17
__Word__    = 32-bit

__Register Description__
      
