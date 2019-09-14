set_property -dict { PACKAGE_PIN T14   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[0] }];
set_property -dict { PACKAGE_PIN U12   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[1] }]; 
set_property -dict { PACKAGE_PIN U13   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[2] }];
set_property -dict { PACKAGE_PIN V13   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[3] }];
set_property -dict { PACKAGE_PIN V15   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[4] }]; 
set_property -dict { PACKAGE_PIN T15   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[5] }];
set_property -dict { PACKAGE_PIN R16   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[6] }];
set_property -dict { PACKAGE_PIN U17   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[7] }]; 
set_property -dict { PACKAGE_PIN V17   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[8] }];
set_property -dict { PACKAGE_PIN V18   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[9] }];
set_property -dict { PACKAGE_PIN T16   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[10] }]; 
set_property -dict { PACKAGE_PIN R17   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[11] }];
set_property -dict { PACKAGE_PIN P18   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[12] }];
set_property -dict { PACKAGE_PIN N17   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[13] }]; 
set_property -dict { PACKAGE_PIN F19   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[14] }];
set_property -dict { PACKAGE_PIN F20   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[15] }];
set_property -dict { PACKAGE_PIN C20   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[16] }]; 
set_property -dict { PACKAGE_PIN B20   IOSTANDARD LVCMOS33 } [get_ports { pwm_out[17] }];

#clk_fpga_0
create_clock -name VCLK -period 10
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[0]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[1]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[2]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[3]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[4]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[5]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[6]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[7]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[8]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[9]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[10]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[11]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[12]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[13]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[14]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[15]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[16]]
set_output_delay 1 -clock [get_clocks VCLK] [get_ports pwm_out[17]]

set_property IOB TRUE [get_ports pwm_out[0]]
set_property IOB TRUE [get_ports pwm_out[1]]
set_property IOB TRUE [get_ports pwm_out[2]]
set_property IOB TRUE [get_ports pwm_out[3]]
set_property IOB TRUE [get_ports pwm_out[4]]
set_property IOB TRUE [get_ports pwm_out[5]]
set_property IOB TRUE [get_ports pwm_out[6]]
set_property IOB TRUE [get_ports pwm_out[7]]
set_property IOB TRUE [get_ports pwm_out[8]]
set_property IOB TRUE [get_ports pwm_out[9]]
set_property IOB TRUE [get_ports pwm_out[10]]
set_property IOB TRUE [get_ports pwm_out[11]]
set_property IOB TRUE [get_ports pwm_out[12]]
set_property IOB TRUE [get_ports pwm_out[13]]
set_property IOB TRUE [get_ports pwm_out[14]]
set_property IOB TRUE [get_ports pwm_out[15]]
set_property IOB TRUE [get_ports pwm_out[16]]
set_property IOB TRUE [get_ports pwm_out[17]]

