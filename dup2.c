#include <sys/stat.h> //open()
#include <fcntl.h>    //open()
#include <stdio.h>    //perror()
#include <errno.h>    //perror()
#include <unistd.h>   //dup2(),close(),write()

int main() {
    int nfd = open("./dup2.txt", O_RDWR|O_CREAT);
    if (nfd == -1) {
        perror("Unable to open /dev/null: ");
        return -1;
    }
    if (dup2(nfd, 0) < 0) {
        perror("Unable to dup2 stdin(0): ");
        return -1;
    }
    if (dup2(nfd, 1) < 0) {
        perror("Unable to dup2 stdout(1): ");
        return -1;
    }
    if (dup2(nfd, 2) < 0) {
        perror("Unable to dup2 stderr(2): ");
        return -1;
    }
    int iRet;
    iRet =close(nfd);
    iRet =close(nfd);
    close(0);
    close(1);
    close(2);
    write(0, "hello world\n", 13);
    write(1, "hello world\n", 13);
    write(2, "hello world\n", 13);
    iRet = close(nfd);
    iRet = close(nfd);
    iRet =close(nfd);

    return 0;
}