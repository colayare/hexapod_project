//Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
//Date        : Thu Jul 30 15:30:21 2020
//Host        : DESKTOP-7SEKLH3 running 64-bit major release  (build 9200)
//Command     : generate_target design_1_wrapper.bd
//Design      : design_1_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module design_1_wrapper
   (DDR_addr,
    DDR_ba,
    DDR_cas_n,
    DDR_ck_n,
    DDR_ck_p,
    DDR_cke,
    DDR_cs_n,
    DDR_dm,
    DDR_dq,
    DDR_dqs_n,
    DDR_dqs_p,
    DDR_odt,
    DDR_ras_n,
    DDR_reset_n,
    DDR_we_n,
    FIXED_IO_ddr_vrn,
    FIXED_IO_ddr_vrp,
    FIXED_IO_mio,
    FIXED_IO_ps_clk,
    FIXED_IO_ps_porb,
    FIXED_IO_ps_srstb,
    IIC_scl_io,
    IIC_sda_io,
    SPI_io0_io,
    SPI_io1_io,
    SPI_sck_io,
    SPI_ss1_o,
    SPI_ss2_o,
    SPI_ss_io,
    pwm_out);
  inout [14:0]DDR_addr;
  inout [2:0]DDR_ba;
  inout DDR_cas_n;
  inout DDR_ck_n;
  inout DDR_ck_p;
  inout DDR_cke;
  inout DDR_cs_n;
  inout [3:0]DDR_dm;
  inout [31:0]DDR_dq;
  inout [3:0]DDR_dqs_n;
  inout [3:0]DDR_dqs_p;
  inout DDR_odt;
  inout DDR_ras_n;
  inout DDR_reset_n;
  inout DDR_we_n;
  inout FIXED_IO_ddr_vrn;
  inout FIXED_IO_ddr_vrp;
  inout [53:0]FIXED_IO_mio;
  inout FIXED_IO_ps_clk;
  inout FIXED_IO_ps_porb;
  inout FIXED_IO_ps_srstb;
  inout IIC_scl_io;
  inout IIC_sda_io;
  inout SPI_io0_io;
  inout SPI_io1_io;
  inout SPI_sck_io;
  output SPI_ss1_o;
  output SPI_ss2_o;
  inout SPI_ss_io;
  output [17:0]pwm_out;

  wire [14:0]DDR_addr;
  wire [2:0]DDR_ba;
  wire DDR_cas_n;
  wire DDR_ck_n;
  wire DDR_ck_p;
  wire DDR_cke;
  wire DDR_cs_n;
  wire [3:0]DDR_dm;
  wire [31:0]DDR_dq;
  wire [3:0]DDR_dqs_n;
  wire [3:0]DDR_dqs_p;
  wire DDR_odt;
  wire DDR_ras_n;
  wire DDR_reset_n;
  wire DDR_we_n;
  wire FIXED_IO_ddr_vrn;
  wire FIXED_IO_ddr_vrp;
  wire [53:0]FIXED_IO_mio;
  wire FIXED_IO_ps_clk;
  wire FIXED_IO_ps_porb;
  wire FIXED_IO_ps_srstb;
  wire IIC_scl_i;
  wire IIC_scl_io;
  wire IIC_scl_o;
  wire IIC_scl_t;
  wire IIC_sda_i;
  wire IIC_sda_io;
  wire IIC_sda_o;
  wire IIC_sda_t;
  wire SPI_io0_i;
  wire SPI_io0_io;
  wire SPI_io0_o;
  wire SPI_io0_t;
  wire SPI_io1_i;
  wire SPI_io1_io;
  wire SPI_io1_o;
  wire SPI_io1_t;
  wire SPI_sck_i;
  wire SPI_sck_io;
  wire SPI_sck_o;
  wire SPI_sck_t;
  wire SPI_ss1_o;
  wire SPI_ss2_o;
  wire SPI_ss_i;
  wire SPI_ss_io;
  wire SPI_ss_o;
  wire SPI_ss_t;
  wire [17:0]pwm_out;

  IOBUF IIC_scl_iobuf
       (.I(IIC_scl_o),
        .IO(IIC_scl_io),
        .O(IIC_scl_i),
        .T(IIC_scl_t));
  IOBUF IIC_sda_iobuf
       (.I(IIC_sda_o),
        .IO(IIC_sda_io),
        .O(IIC_sda_i),
        .T(IIC_sda_t));
  IOBUF SPI_io0_iobuf
       (.I(SPI_io0_o),
        .IO(SPI_io0_io),
        .O(SPI_io0_i),
        .T(SPI_io0_t));
  IOBUF SPI_io1_iobuf
       (.I(SPI_io1_o),
        .IO(SPI_io1_io),
        .O(SPI_io1_i),
        .T(SPI_io1_t));
  IOBUF SPI_sck_iobuf
       (.I(SPI_sck_o),
        .IO(SPI_sck_io),
        .O(SPI_sck_i),
        .T(SPI_sck_t));
  IOBUF SPI_ss_iobuf
       (.I(SPI_ss_o),
        .IO(SPI_ss_io),
        .O(SPI_ss_i),
        .T(SPI_ss_t));
  design_1 design_1_i
       (.DDR_addr(DDR_addr),
        .DDR_ba(DDR_ba),
        .DDR_cas_n(DDR_cas_n),
        .DDR_ck_n(DDR_ck_n),
        .DDR_ck_p(DDR_ck_p),
        .DDR_cke(DDR_cke),
        .DDR_cs_n(DDR_cs_n),
        .DDR_dm(DDR_dm),
        .DDR_dq(DDR_dq),
        .DDR_dqs_n(DDR_dqs_n),
        .DDR_dqs_p(DDR_dqs_p),
        .DDR_odt(DDR_odt),
        .DDR_ras_n(DDR_ras_n),
        .DDR_reset_n(DDR_reset_n),
        .DDR_we_n(DDR_we_n),
        .FIXED_IO_ddr_vrn(FIXED_IO_ddr_vrn),
        .FIXED_IO_ddr_vrp(FIXED_IO_ddr_vrp),
        .FIXED_IO_mio(FIXED_IO_mio),
        .FIXED_IO_ps_clk(FIXED_IO_ps_clk),
        .FIXED_IO_ps_porb(FIXED_IO_ps_porb),
        .FIXED_IO_ps_srstb(FIXED_IO_ps_srstb),
        .IIC_scl_i(IIC_scl_i),
        .IIC_scl_o(IIC_scl_o),
        .IIC_scl_t(IIC_scl_t),
        .IIC_sda_i(IIC_sda_i),
        .IIC_sda_o(IIC_sda_o),
        .IIC_sda_t(IIC_sda_t),
        .SPI_io0_i(SPI_io0_i),
        .SPI_io0_o(SPI_io0_o),
        .SPI_io0_t(SPI_io0_t),
        .SPI_io1_i(SPI_io1_i),
        .SPI_io1_o(SPI_io1_o),
        .SPI_io1_t(SPI_io1_t),
        .SPI_sck_i(SPI_sck_i),
        .SPI_sck_o(SPI_sck_o),
        .SPI_sck_t(SPI_sck_t),
        .SPI_ss1_o(SPI_ss1_o),
        .SPI_ss2_o(SPI_ss2_o),
        .SPI_ss_i(SPI_ss_i),
        .SPI_ss_o(SPI_ss_o),
        .SPI_ss_t(SPI_ss_t),
        .pwm_out(pwm_out));
endmodule
