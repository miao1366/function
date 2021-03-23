#include <dirent.h>
#include <stdio.h>

int main(void) {
    DIR *dirptr = NULL;
    int i = 1;
    struct dirent *entry;
    if ((dirptr = opendir("/usr/local/etc/scsensord/")) == NULL) {
        printf("opendir failed!");
        return 1;
    } else {
        while (entry = readdir(dirptr)) {
            printf("filename%d=%s\n", i, entry->d_name);
            i++;
        }
        closedir(dirptr);
    }
    return 0;
}