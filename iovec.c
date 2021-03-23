#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>



// int main(){
// 	struct iovec iov[3];
// 	ssize_t nr;
// 	int fd,i;
	
// 	char *buf[] = {
// 		"The term buccaneer comes from the word boucan.\n",
// 		"A boucan is a wooden frame used for cooking meat.\n",
// 		"Buccaneer is the West Indies name for a pirate.\n" 
// 	};
	
// 	fd = open("buccaneer.txt",O_WRONLY | O_CREAT | O_TRUNC);
// 	if( fd == -1 ){
// 		perror("open");
// 		return 1;
// 	}

// 	for(i = 0; i < 3; i++){
// 		iov[i].iov_base = buf[i];
// 		iov[i].iov_len = strlen(buf[i]);
// 	}
	
// 	nr = writev(fd,iov,3);
// 	if(nr == -1){
// 		perror("writev");
// 		return 1;
// 	}
// 	printf("wrote %ld bytes\n",nr);

// 	if(close(fd)){
// 		perror("close");
// 		return 1;
// 	}
// 	return 0;
// }


int main(){
	char foo[48],bar[51],baz[49];
	struct iovec iov[3];
	ssize_t nr;
	int fd,i;

	fd = open("buccaneer.txt",O_RDONLY);
	if(fd == -1){
		perror("open");
		return 1;
	}

	iov[0].iov_base = foo;
	iov[0].iov_len = sizeof(foo);
	iov[1].iov_base = bar;
	iov[1].iov_len = sizeof(bar);
	iov[2].iov_base = baz;
	iov[2].iov_len = sizeof(baz);

	nr = readv(fd,iov,3);
	if(nr == -1){
		perror("readv");
		return 1;
	}

	for(i = 0; i < 3; i++){
		printf("%d: %s",i,(char *)iov[i].iov_base);
	}

	if(close(fd)){
		perror("close");
		return 1;
	}
	return 0;
}
