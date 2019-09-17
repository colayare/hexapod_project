#ifndef AXI_ACCESS_H
#define AXI_ACCESS_H

// Typedefs 
typedef struct ik_input_t {
    int x_in;
    int y_in;
    int z_in;
} ik_input_t;

typedef struct ik_output_t {
    int q1_out;
    int q2_out;
    int q3_out;
} ik_output_t;

// Functions declarations
extern int *init_axi_ikinematics();
extern int axi_read(int *axi_mmap, int read_address);
extern void axi_write(int *axi_mmap, int write_address, int value);
extern void axi_write_fifo(int *axi_mmap);
extern void axi_trigger_ikinematics(int *axi_mmap);
extern void axi_leg_config(int *axi_mmap, int conf, int leg_select);
extern void axi_show_all_regs(int *axi_mmap);
extern ik_output_t axi_ikinematics_results(int *axi_mmap);
extern void axi_ik_write_gait_step(int *axi_mmap, ik_input_t gait_step);


#endif // AXI_ACCESS_H