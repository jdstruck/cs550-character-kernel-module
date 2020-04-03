#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
//	printf("hello\n");	
	char buf[256];
	int intbuf[4];
	int fscanf_output = 0;
	int fd = open("/dev/dev_ps", O_RDWR);
	//FILE *ret = fopen("/dev/dev_ps", "r");
	if (fd < 0) {
		perror("failed to open /dev/dev_ps");
		exit(1);
	}
 	printf("fd = %d\n", fd);
 
	//while((fscanf_output == fscanf(ret, "%s", buf)) != EOF ) {
	//	printf("%s\n", buf);
	//}
	//fgets(buf, 256, ret);
	//printf("%s\n", buf);
	int bytes_read = read(fd, buf, 256);
	if(bytes_read < 0) printf("nothing read\n");
	//for(int i = 0; i < 4; ++i)
	//	printf("intbuf[%d]: %d\n", i, intbuf[i]);
	//printf("\n");
	printf("\n%s\nbytes read: %d\n", buf, bytes_read);
	if(close(fd) < 0) {
		perror("close error");
		exit(1);
	}
	printf("fclosed ret\n");
	return 0;
}

