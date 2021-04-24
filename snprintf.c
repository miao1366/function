#include <stdio.h>
#include <string.h>

int main()  
{  
  char buf[10] = {0};  
  char src[30] = "hello world! hello world!";  
  int len = snprintf(buf, sizeof(buf), "%s", src);  
  printf("return len=%d\n", len);  
  buf[len] = '\0';  
  printf("buf=%s, bufLen=%ld\n", buf, strlen(buf));  
  return 0;  
}  
