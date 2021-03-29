#include <unistd.h>
#include <stdio.h>

int main() {

    printf("配置的处理器数量为 :%ld\n", sysconf(_SC_NPROCESSORS_CONF));                //1
    printf("当前在线(可用)的处理器数量为 :%ld\n", sysconf(_SC_NPROCESSORS_ONLN));       //1
    printf ("页大小: %ld\n", sysconf(_SC_PAGESIZE));                                //4096
    printf ("页数: %ld\n", sysconf(_SC_PHYS_PAGES));                                //248933  
    printf ("可用页面的数量: %ld\n", sysconf(_SC_AVPHYS_PAGES));                      //17336
    printf ("内存大小: %lld MB\n", (long long)sysconf(_SC_PAGESIZE) * (long long)sysconf(_SC_PHYS_PAGES));  
    printf ("最大打开的文件数: %ld\n", sysconf(_SC_OPEN_MAX));                        //1024
    printf("每秒的节拍数: %ld\n", sysconf(_SC_CLK_TCK));                             //100
    printf ("主机名的最大长度: %ld\n", sysconf(_SC_HOST_NAME_MAX));                   //64
    printf ("登录名的最大长度: %ld\n", sysconf(_SC_LOGIN_NAME_MAX));                  //256 
    return 0;

}