/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
//#include "ps7_init.h"
#include "sleep.h"

void write_reg(int ptr, int value)
{
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR + 4*ptr, value);
}

void write_fifo()
{
	
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000001);
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000000);
}

int read_fifo(int ptr)
{
	int value;
	value = Xil_In32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR + ptr*4);
	return value;
}

void trigger_ikinematics()
{
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000002);
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000000);
}

void write_start_ptr(int ptr)
{
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR + 4, ptr);
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000004);
	Xil_Out32(XPAR_AXI_IKINEMATICS_0_S00_AXI_BASEADDR, 0x00000000);
}

void write_offset(int os_q1, int os_q2, int os_q3, int start_ptr)
{
	int start_addr = 5 + start_ptr;
	xil_printf("Writing offset ptr %x : %x - %x - %x\n\r", start_addr, os_q1, os_q2, os_q3);
	write_reg(start_addr, os_q1);
	write_reg(start_addr + 1, os_q2);
	write_reg(start_addr + 2, os_q3);
}

float sec2rad(float insec)
{
	const float pi_val = 3.14159;
	return insec * pi_val / 180;
}

int calculate_offset(float offset)
{
	const float pi_val = 3.14159;
	float offset1 = pi_val/4 + sec2rad(offset);
	int hexOffset1 = * (int *)&offset1;

	return hexOffset1;
}

void show_all_registers()
{
	int readVal = 0;
    for (int j=0; j<40; j++)
    {
    	readVal = read_fifo(j);
    	xil_printf("Step %2x:\t%x\n\r", j, readVal);
    }
}


int main()
{
	int pwm1, pwm2, pwm3;
	int x_input [120] = {0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfdf3b6,0x3dfd8ada,0x3df65fd8,0x3def34d6,0x3de809d4,0x3de0ded2,0x3dd9b3d0,0x3dd288ce,0x3dcb5dcc,0x3dc432ca,0x3dbd07c8,0x3db5dcc6,0x3db98c7e,0x3dbd07c8,0x3dc0b780,0x3dc432ca,0x3dc7e282,0x3dcb5dcc,0x3dcf0d84,0x3dd288ce,0x3dd63886,0x3dd9b3d0,0x3ddd6388,0x3de0ded2,0x3de48e8a,0x3de809d4,0x3debb98c,0x3def34d6,0x3df2e48e,0x3df65fd8,0x3dfa0f90,0x3dfd8ada,0x3df65fd8,0x3def34d6,0x3de809d4,0x3de0ded2,0x3dd9b3d0,0x3dd288ce,0x3dcb5dcc,0x3dc432ca,0x3dbd07c8,0x3db5dcc6,0x3db98c7e,0x3dbd07c8,0x3dc0b780,0x3dc432ca,0x3dc7e282,0x3dcb5dcc,0x3dcf0d84,0x3dd288ce,0x3dd63886,0x3dd9b3d0,0x3ddd6388,0x3de0ded2,0x3de48e8a,0x3de809d4,0x3debb98c,0x3def34d6,0x3df2e48e,0x3df65fd8,0x3dfa0f90,0x3dfd8ada,0x3dfec56d,0x3dffcb92,0x3e008312,0x3e01205b,0x3e01bda5,0x3e0240b7,0x3e02de00,0x3e037b4a,0x3e041893,0x3e049ba5,0x3e01bda5,0x3dfd8ada,0x3df79a6b,0x3df1de69,0x3debedfa,0x3ded288c,0x3dee2eb1,0x3def6944,0x3df0a3d7,0x3df1de69,0x3df2e48e,0x3df41f21,0x3df559b3,0x3df69446,0x3df79a6b,0x3df8d4fd,0x3dfa0f90,0x3dfb4a23,0x3dfc5048};
	int y_input [120] = {0x3d4ccccc,0x3d54fdf3,0x3d591687,0x3d6147ae,0x3d6978d4,0x3d6d9168,0x3d75c28f,0x3d7df3b6,0x3d810624,0x3d851eb8,0x3d89374b,0x3d8b4395,0x3d8f5c28,0x3d9374bc,0x3d958106,0x3d999999,0x3d958106,0x3d9374bc,0x3d8f5c28,0x3d8b4395,0x3d89374b,0x3d851eb8,0x3d810624,0x3d7df3b6,0x3d75c28f,0x3d6d9168,0x3d6978d4,0x3d6147ae,0x3d591687,0x3d54fdf3,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3cf5c28f,0x3d23d70a,0x3d20902d,0x3d1d4951,0x3d199999,0x3d1652bd,0x3d130be0,0x3d0fc504,0x3d0c7e28,0x3d08ce70,0x3d058793,0x3d0240b7,0x3d130be0,0x3d23d70a,0x3d34a233,0x3d456d5c,0x3d563886,0x3d52f1a9,0x3d4f41f2,0x3d4bfb15,0x3d48b439,0x3d456d5c,0x3d422680,0x3d3edfa4,0x3d3b2fec,0x3d37e90f,0x3d34a233,0x3d315b57,0x3d2e147a,0x3d2a64c2,0x3d271de6};
	int z_input [120] = {0xbdd70a3d,0xbdcac083,0xbdc08312,0xbdb645a1,0xbdb020c4,0xbda9fbe7,0xbda5e353,0xbda3d70a,0xbda3d70a,0xbda5e353,0xbda9fbe7,0xbdb020c4,0xbdb645a1,0xbdc08312,0xbdcac083,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd70a3d,0xbdd73eab,0xbdbd70a3,0xbda95e9e,0xbd9b089a,0xbd926e97,0xbd8f9096,0xbd926e97,0xbd9b089a,0xbda95e9e,0xbdbd70a3,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdbd70a3,0xbda95e9e,0xbd9b089a,0xbd926e97,0xbd8f9096,0xbd926e97,0xbd9b089a,0xbda95e9e,0xbdbd70a3,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdb67a0f,0xbda617c1,0xbda617c1,0xbdb67a0f,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab,0xbdd73eab};

	float offsets [6][3] = { \
								{-6, 40, 150}, \
								{-6, 40, 150}, \
								{-6, 40, 150}, \
								{-6, 40, 150}, \
								{-6, 40, 150}, \
								{-6, 40, 150} \
								};

	init_platform();
	usleep(100);
	print("Test iKinematics\n\r");
	
	write_start_ptr(0);
	for (int iii=0; iii<6; iii++)
    {
    	write_offset(calculate_offset(offsets[iii][0]), calculate_offset(offsets[iii][1]), calculate_offset(offsets[iii][2]) , iii*3);
    }

	write_start_ptr(0);

	//show_all_registers();
	usleep(50);


	for (int j=0; j<1; j++)
	{
		// Set X, Y, Z
		for (int ii=0; ii<1; ii++)
		{

			for (int i; i<6; i++)
			{
				write_reg(2, x_input[ii]);
				write_reg(3, y_input[ii]);
				write_reg(4, z_input[ii]);
				write_start_ptr(0);
			}

			trigger_ikinematics();
			usleep(10);
			
			show_all_registers();
			/*pwm1 = read_fifo(0x17);
			pwm2 = read_fifo(0x18);
			pwm3 = read_fifo(0x19);
			xil_printf("Step %3d:\t%d\t%d\t%d\n\r", ii, pwm1, pwm2, pwm3);*/

			usleep(33400);
		}
	}

   cleanup_platform();
   return 0;

}
