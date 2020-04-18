#ifndef IKINEMATICS_IP_CONTEXT
#define IKINEMATICS_IP_CONTEXT

struct AXI_IKINEMATICS_CONTEXT {
    // IP Context Information
    IP_CONTEXT_t context;
    
    // AXI IP Function Pointers
    int32_t (*axi_read) (context.
};

int (*create_ikinematics_ip_mmap) (IP_CONTEXT_t);

AXI_IKINEMATICS_CONTEXT *init_axi_ikinematics() {
    AXI_IKINEMATICS_CONTEXT ikinematics_ip_context;
    ikinematics_ip_context->CONTEXT = AXI_IKINEMATICS_
}

#define 

#endif // IKINEMATICS_IP_CONTEXT