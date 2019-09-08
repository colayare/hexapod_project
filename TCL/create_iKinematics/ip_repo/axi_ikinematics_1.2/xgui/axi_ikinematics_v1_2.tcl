# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  set C_S00_AXI_DATA_WIDTH [ipgui::add_param $IPINST -name "C_S00_AXI_DATA_WIDTH" -parent ${Page_0} -widget comboBox]
  set_property tooltip {Width of S_AXI data bus} ${C_S00_AXI_DATA_WIDTH}
  set C_S00_AXI_ADDR_WIDTH [ipgui::add_param $IPINST -name "C_S00_AXI_ADDR_WIDTH" -parent ${Page_0}]
  set_property tooltip {Width of S_AXI address bus} ${C_S00_AXI_ADDR_WIDTH}
  ipgui::add_param $IPINST -name "C_S00_AXI_BASEADDR" -parent ${Page_0}
  ipgui::add_param $IPINST -name "C_S00_AXI_HIGHADDR" -parent ${Page_0}


}

proc update_PARAM_VALUE.C_COR_ITER { PARAM_VALUE.C_COR_ITER } {
	# Procedure called to update C_COR_ITER when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_COR_ITER { PARAM_VALUE.C_COR_ITER } {
	# Procedure called to validate C_COR_ITER
	return true
}

proc update_PARAM_VALUE.C_COR_N_ITER { PARAM_VALUE.C_COR_N_ITER } {
	# Procedure called to update C_COR_N_ITER when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_COR_N_ITER { PARAM_VALUE.C_COR_N_ITER } {
	# Procedure called to validate C_COR_N_ITER
	return true
}

proc update_PARAM_VALUE.C_FIFO_DEPTH { PARAM_VALUE.C_FIFO_DEPTH } {
	# Procedure called to update C_FIFO_DEPTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FIFO_DEPTH { PARAM_VALUE.C_FIFO_DEPTH } {
	# Procedure called to validate C_FIFO_DEPTH
	return true
}

proc update_PARAM_VALUE.C_FLP_WIDTH { PARAM_VALUE.C_FLP_WIDTH } {
	# Procedure called to update C_FLP_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FLP_WIDTH { PARAM_VALUE.C_FLP_WIDTH } {
	# Procedure called to validate C_FLP_WIDTH
	return true
}

proc update_PARAM_VALUE.C_FXP_POINT { PARAM_VALUE.C_FXP_POINT } {
	# Procedure called to update C_FXP_POINT when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FXP_POINT { PARAM_VALUE.C_FXP_POINT } {
	# Procedure called to validate C_FXP_POINT
	return true
}

proc update_PARAM_VALUE.C_FXP_WIDTH { PARAM_VALUE.C_FXP_WIDTH } {
	# Procedure called to update C_FXP_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_FXP_WIDTH { PARAM_VALUE.C_FXP_WIDTH } {
	# Procedure called to validate C_FXP_WIDTH
	return true
}

proc update_PARAM_VALUE.C_IKN_C2 { PARAM_VALUE.C_IKN_C2 } {
	# Procedure called to update C_IKN_C2 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_C2 { PARAM_VALUE.C_IKN_C2 } {
	# Procedure called to validate C_IKN_C2
	return true
}

proc update_PARAM_VALUE.C_IKN_C3 { PARAM_VALUE.C_IKN_C3 } {
	# Procedure called to update C_IKN_C3 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_C3 { PARAM_VALUE.C_IKN_C3 } {
	# Procedure called to validate C_IKN_C3
	return true
}

proc update_PARAM_VALUE.C_IKN_Ca8 { PARAM_VALUE.C_IKN_Ca8 } {
	# Procedure called to update C_IKN_Ca8 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_Ca8 { PARAM_VALUE.C_IKN_Ca8 } {
	# Procedure called to validate C_IKN_Ca8
	return true
}

proc update_PARAM_VALUE.C_IKN_F { PARAM_VALUE.C_IKN_F } {
	# Procedure called to update C_IKN_F when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_F { PARAM_VALUE.C_IKN_F } {
	# Procedure called to validate C_IKN_F
	return true
}

proc update_PARAM_VALUE.C_IKN_L1 { PARAM_VALUE.C_IKN_L1 } {
	# Procedure called to update C_IKN_L1 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_L1 { PARAM_VALUE.C_IKN_L1 } {
	# Procedure called to validate C_IKN_L1
	return true
}

proc update_PARAM_VALUE.C_IKN_iK { PARAM_VALUE.C_IKN_iK } {
	# Procedure called to update C_IKN_iK when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_iK { PARAM_VALUE.C_IKN_iK } {
	# Procedure called to validate C_IKN_iK
	return true
}

proc update_PARAM_VALUE.C_IKN_iKH { PARAM_VALUE.C_IKN_iKH } {
	# Procedure called to update C_IKN_iKH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_iKH { PARAM_VALUE.C_IKN_iKH } {
	# Procedure called to validate C_IKN_iKH
	return true
}

proc update_PARAM_VALUE.C_IKN_nL1 { PARAM_VALUE.C_IKN_nL1 } {
	# Procedure called to update C_IKN_nL1 when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_IKN_nL1 { PARAM_VALUE.C_IKN_nL1 } {
	# Procedure called to validate C_IKN_nL1
	return true
}

proc update_PARAM_VALUE.C_PWM_FREQ { PARAM_VALUE.C_PWM_FREQ } {
	# Procedure called to update C_PWM_FREQ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_PWM_FREQ { PARAM_VALUE.C_PWM_FREQ } {
	# Procedure called to validate C_PWM_FREQ
	return true
}

proc update_PARAM_VALUE.C_PWM_MAX_IN { PARAM_VALUE.C_PWM_MAX_IN } {
	# Procedure called to update C_PWM_MAX_IN when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_PWM_MAX_IN { PARAM_VALUE.C_PWM_MAX_IN } {
	# Procedure called to validate C_PWM_MAX_IN
	return true
}

proc update_PARAM_VALUE.C_PWM_OFFSET { PARAM_VALUE.C_PWM_OFFSET } {
	# Procedure called to update C_PWM_OFFSET when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_PWM_OFFSET { PARAM_VALUE.C_PWM_OFFSET } {
	# Procedure called to validate C_PWM_OFFSET
	return true
}

proc update_PARAM_VALUE.C_PWM_SIZE { PARAM_VALUE.C_PWM_SIZE } {
	# Procedure called to update C_PWM_SIZE when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_PWM_SIZE { PARAM_VALUE.C_PWM_SIZE } {
	# Procedure called to validate C_PWM_SIZE
	return true
}

proc update_PARAM_VALUE.C_ROB_NJOINTS { PARAM_VALUE.C_ROB_NJOINTS } {
	# Procedure called to update C_ROB_NJOINTS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_ROB_NJOINTS { PARAM_VALUE.C_ROB_NJOINTS } {
	# Procedure called to validate C_ROB_NJOINTS
	return true
}

proc update_PARAM_VALUE.C_ROB_NLEGS { PARAM_VALUE.C_ROB_NLEGS } {
	# Procedure called to update C_ROB_NLEGS when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_ROB_NLEGS { PARAM_VALUE.C_ROB_NLEGS } {
	# Procedure called to validate C_ROB_NLEGS
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.C_FIFO_DEPTH { MODELPARAM_VALUE.C_FIFO_DEPTH PARAM_VALUE.C_FIFO_DEPTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FIFO_DEPTH}] ${MODELPARAM_VALUE.C_FIFO_DEPTH}
}

proc update_MODELPARAM_VALUE.C_FLP_WIDTH { MODELPARAM_VALUE.C_FLP_WIDTH PARAM_VALUE.C_FLP_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FLP_WIDTH}] ${MODELPARAM_VALUE.C_FLP_WIDTH}
}

proc update_MODELPARAM_VALUE.C_FXP_WIDTH { MODELPARAM_VALUE.C_FXP_WIDTH PARAM_VALUE.C_FXP_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FXP_WIDTH}] ${MODELPARAM_VALUE.C_FXP_WIDTH}
}

proc update_MODELPARAM_VALUE.C_FXP_POINT { MODELPARAM_VALUE.C_FXP_POINT PARAM_VALUE.C_FXP_POINT } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_FXP_POINT}] ${MODELPARAM_VALUE.C_FXP_POINT}
}

proc update_MODELPARAM_VALUE.C_IKN_iK { MODELPARAM_VALUE.C_IKN_iK PARAM_VALUE.C_IKN_iK } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_iK}] ${MODELPARAM_VALUE.C_IKN_iK}
}

proc update_MODELPARAM_VALUE.C_IKN_iKH { MODELPARAM_VALUE.C_IKN_iKH PARAM_VALUE.C_IKN_iKH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_iKH}] ${MODELPARAM_VALUE.C_IKN_iKH}
}

proc update_MODELPARAM_VALUE.C_IKN_F { MODELPARAM_VALUE.C_IKN_F PARAM_VALUE.C_IKN_F } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_F}] ${MODELPARAM_VALUE.C_IKN_F}
}

proc update_MODELPARAM_VALUE.C_IKN_C2 { MODELPARAM_VALUE.C_IKN_C2 PARAM_VALUE.C_IKN_C2 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_C2}] ${MODELPARAM_VALUE.C_IKN_C2}
}

proc update_MODELPARAM_VALUE.C_IKN_C3 { MODELPARAM_VALUE.C_IKN_C3 PARAM_VALUE.C_IKN_C3 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_C3}] ${MODELPARAM_VALUE.C_IKN_C3}
}

proc update_MODELPARAM_VALUE.C_IKN_Ca8 { MODELPARAM_VALUE.C_IKN_Ca8 PARAM_VALUE.C_IKN_Ca8 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_Ca8}] ${MODELPARAM_VALUE.C_IKN_Ca8}
}

proc update_MODELPARAM_VALUE.C_IKN_L1 { MODELPARAM_VALUE.C_IKN_L1 PARAM_VALUE.C_IKN_L1 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_L1}] ${MODELPARAM_VALUE.C_IKN_L1}
}

proc update_MODELPARAM_VALUE.C_IKN_nL1 { MODELPARAM_VALUE.C_IKN_nL1 PARAM_VALUE.C_IKN_nL1 } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_IKN_nL1}] ${MODELPARAM_VALUE.C_IKN_nL1}
}

proc update_MODELPARAM_VALUE.C_COR_ITER { MODELPARAM_VALUE.C_COR_ITER PARAM_VALUE.C_COR_ITER } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_COR_ITER}] ${MODELPARAM_VALUE.C_COR_ITER}
}

proc update_MODELPARAM_VALUE.C_COR_N_ITER { MODELPARAM_VALUE.C_COR_N_ITER PARAM_VALUE.C_COR_N_ITER } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_COR_N_ITER}] ${MODELPARAM_VALUE.C_COR_N_ITER}
}

proc update_MODELPARAM_VALUE.C_PWM_OFFSET { MODELPARAM_VALUE.C_PWM_OFFSET PARAM_VALUE.C_PWM_OFFSET } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_PWM_OFFSET}] ${MODELPARAM_VALUE.C_PWM_OFFSET}
}

proc update_MODELPARAM_VALUE.C_PWM_SIZE { MODELPARAM_VALUE.C_PWM_SIZE PARAM_VALUE.C_PWM_SIZE } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_PWM_SIZE}] ${MODELPARAM_VALUE.C_PWM_SIZE}
}

proc update_MODELPARAM_VALUE.C_PWM_FREQ { MODELPARAM_VALUE.C_PWM_FREQ PARAM_VALUE.C_PWM_FREQ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_PWM_FREQ}] ${MODELPARAM_VALUE.C_PWM_FREQ}
}

proc update_MODELPARAM_VALUE.C_PWM_MAX_IN { MODELPARAM_VALUE.C_PWM_MAX_IN PARAM_VALUE.C_PWM_MAX_IN } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_PWM_MAX_IN}] ${MODELPARAM_VALUE.C_PWM_MAX_IN}
}

proc update_MODELPARAM_VALUE.C_ROB_NLEGS { MODELPARAM_VALUE.C_ROB_NLEGS PARAM_VALUE.C_ROB_NLEGS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_ROB_NLEGS}] ${MODELPARAM_VALUE.C_ROB_NLEGS}
}

proc update_MODELPARAM_VALUE.C_ROB_NJOINTS { MODELPARAM_VALUE.C_ROB_NJOINTS PARAM_VALUE.C_ROB_NJOINTS } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_ROB_NJOINTS}] ${MODELPARAM_VALUE.C_ROB_NJOINTS}
}

