# Hexapod Project
The Hexapod Project is an Open Source Hexapod Robot Hardware Platform based on a Xilinx Zynq-7000 System-on-Chip Device. The aim is to provide a full-customizable processing environment for an Hexapod Robot.\
The initial idea published on _Design and Modeling of a Research Platform based on Hexapod Robot with Embedded Systems and Interactive Control_ [[1]](#publications) presents the Robot Mechanics, Kinematics & Locomotion Control Model. In this first stage, all Hexapod Kinematics Equations was solved by main the processor. However, this complex equations involves complex trigonometics, square root and trascendental functions in addition to being redundant because must be calculated for each robot leg. This implies high computational cost that can be used to focus in more important and dynamic tasks such as image processing or trajectory planning. Thus, the approach presented on _Fully-pipelined CORDIC-based FPGA Realization for a 3-DOF Hexapod-Leg Inverse Kinematics Calculaiton_ [[2]](#publications) proposes an CORDIC-based Pipeline architecture in order to solve these equations concurrently in a FPGA Digital IP.\
In the present, the processing platform has been migrated to a Zynq-7000 System-on-Chip where the Inverse Kinematics IP is located on Programmable Logic and the Locomotion calculation is performed in Software on the Processing System. 
The next step is to develope the Application Layer which involves computational vision to perform trajectory planning.
 

## Prerequisites
#### Hardware
This project is based on [Arty Z7-7010](https://store.digilentinc.com/arty-z7-apsoc-zynq-7000-development-board-for-makers-and-hobbyists/) board by Digilent. A expansion board for CSI interface and Servo Signal-adaptation board is presented in Electronics Design.

#### Tools
|Tool|Version|OS|Source|
|------|------|------|------|
|Vivado|2018.3 and later| Windows, Linux|[Xilinx](https://www.xilinx.com/products/design-tools/vivado.html)|
|Petalinux Tools||Ubuntu 16.04 LTS|[Digilent Github](https://github.com/Digilent/Petalinux-Arty-Z7-10?fbclid=IwAR1qblnWDQWKo1OTBELzMO_X1l97VDp9vJyvZOK7HC7u8QE7sqmfdpN6cZw)|
|Python|3 and later|Windows, Linux|[Anaconda.org](https://anaconda.org/)|

## Hardware Design
### IP Developement
**Inverse Kinematics IP**
 - [Documentation](Vivado/Doc/inverse_kinematics_ip.md)
 - [RTL](Vivado/IP/)

### Electronics Design
**Arty Z7-7010 Board Expansion**
 - Under construction

### Mechanic Design
 - Under construction.

## Deployment 

1. Hardware Deploying\
[Vivado Environment Setup & Deploy](Vivado/README.md)

2. Petalinux Deploying\
[Petalinux Deploy](Petalinux/README.md)

3. Electronics Design

4. First Tests


## Video Documentation
[Hexapod Project Youtube Channel](https://www.youtube.com/channel/UCVvloneIRCm_nlw63EyGIQw)

## Authors
* **Carlos Roger Olaya Reyes** - _SoC Integration Design, Digital IP Design, Initial Work_ - [colayare](https://github.com/colayare)
* **Guillermo Evangelista Adrianzen** - _Robotics Modeling, Initial Work_ - [Robo3001-design](https://github.com/Robo3001-design)
* **Erick Rodriguez Davila** - _Electronics Design, Mechanical Design, Initial Work_
* **Mauricio Gardini Obando** - _Electronics Design_

## Publications
**[1]** G. Evangelista, "Design and Modeling of a Mobile Research Platform based on Hexapod Robot with Embedded System and Interactive Control", 23rd International Conference on Methods and Models in Automation and Robotics, 2014.\
**[2]** Guillermo Evangelista, Carlos Olaya, Erick Rodríguez, "Fully-pipelined CORDIC-based FPGA Realization for a 3-DOF Hexapod-Leg Inverse Kinematics Calculation", Advanced Robotics and Automation (WRC SARA) 2018 WRC Symposium.
