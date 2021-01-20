#include "./cJSON.h"
#include <stdio.h>

int main() {
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", array);
    cJSON *strItem = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, strItem);
    strItem = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, strItem);
    strItem = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, strItem);
    strItem = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, strItem);
    printf("%s\n", cJSON_PrintUnformatted(root));
    
    cJSON *pstCJsonRoot = cJSON_CreateObject();
    if (NULL == pstCJsonRoot) {
        return 0;
    }
    double f = 0.18899999558925629;
    double f1 = 0.0000000000144;
    double f2 = 1889999.9558925629;
    double f3 = 1.8899999558925629;
    cJSON_AddNumberToObject(pstCJsonRoot, "f",f);
    cJSON_AddNumberToObject(pstCJsonRoot, "f1",f1);
    cJSON_AddNumberToObject(pstCJsonRoot, "f2",f2);
    cJSON_AddNumberToObject(pstCJsonRoot, "f3",f3);
    cJSON_AddNumberToObject(pstCJsonRoot, "i1",-1);
    cJSON_AddNumberToObject(pstCJsonRoot, "i2",-100ll);
    cJSON_AddNumberToObject(pstCJsonRoot, "i3",1ul);
    char *pcData = cJSON_PrintUnformatted(pstCJsonRoot);
    printf("%s\n", pcData);

    printf("0.18899999558925629:: %2f\n",f);
    printf("0.0000000000144::     %2f\n",f1);
    printf("1889999.9558925629::  %2f\n",f2);
    printf("1.8899999558925629::  %2f\n",f3);
    
    return 0;
}