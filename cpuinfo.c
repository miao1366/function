#include <stdio.h>
#include <string.h>
#include <stdint.h>

static  void
ngx_cpuid(uint32_t i, uint32_t *buf)
{
    uint32_t  eax, ebx, ecx, edx;
    __asm__ (

        "cpuid"

    : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (i) );

    buf[0] = eax;
    buf[1] = ebx;
    buf[2] = edx;
    buf[3] = ecx;
}

typedef uintptr_t       ngx_uint_t;
ngx_uint_t ngx_cacheline_size;
void ngx_cpuinfo(void)
{
    char    *vendor;
    char    *ven;
    uint32_t   vbuf[5], cpu[4], model;

    vbuf[0] = 0;
    vbuf[1] = 0;
    vbuf[2] = 0;
    vbuf[3] = 0;
    vbuf[4] = 0;
    ngx_cpuid(0, vbuf);
    vendor = (char *) &vbuf[1];
    ven = (char *) &vbuf[0];
    printf("vendor:%s\n", vendor);
    printf("ven:%s\n", ven);
    if (vbuf[0] == 0) {
        return;
    }
    ngx_cpuid(1, cpu);
    printf("vendor2:%s\n", vendor);
    if (strcmp(vendor, "GenuineIntel") == 0) {
        switch ((cpu[0] & 0xf00) >> 8) {
        case 5:
            ngx_cacheline_size = 32;
            break;
        case 6:
            ngx_cacheline_size = 32;
            model = ((cpu[0] & 0xf0000) >> 8) | (cpu[0] & 0xf0);
            if (model >= 0xd0) {
                ngx_cacheline_size = 64;
            }

            break;
        case 15:
            ngx_cacheline_size = 128;
            break;
        }

    } else if (strcmp(vendor, "AuthenticAMD") == 0) {
        ngx_cacheline_size = 64;
    }
}



int main(){
    ngx_cpuinfo();
    return 0;
}
