#include <time.h>    //localtiem_r()  time()
#include <stdio.h>   //sprintf()

int main(void) {
    time_t time_ = time(NULL);
    struct  tm Tm = {0};
    localtime_r(&time_, &Tm);
    char buff[32] = {0};
    sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d",Tm.tm_yday+1900,Tm.tm_mon+1,
                                                Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec);
    
    printf("%s\n", buff);
    return 1;
 }