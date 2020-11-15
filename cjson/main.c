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

}