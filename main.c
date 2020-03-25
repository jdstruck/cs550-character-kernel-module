#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	printf("hello\n");	
	int fd = open("/dev/mymod", O_RDWR);
	if(fd < 0) {
		perror("failed to open /dev/dev_ps");
		exit(1);
	}
	printf("fd = %d\n", fd);

	if(close(fd) < 0) {
		perror("close error");
		exit(1);
	}
	printf("closed fd\n");
	return 0;
}

