#include <stdint.h>
#include <time.h>

uint32_t delay_ms ( uint32_t ms ) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = ms *1000*1000;
    nanosleep(&req, (struct timespec *)NULL);
    
    return 1;
}

uint32_t delay_us ( uint32_t us ) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = us * 1000;
    nanosleep(&req, (struct timespec *)NULL);
    
    return 1;
}