#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 20000

int main() {
	char *buf0 = calloc(BUFSIZE, 1);
	char *buf1 = calloc(BUFSIZE, 1);
	int fd0 = open("/dev/dev_ps", O_RDWR);
	int fd1 = open("/dev/dev_ps", O_RDWR);
	if (fd0 < 0) {
		perror("failed to open /dev/dev_ps");
		exit(1);
	}
	if (fd1 < 0) {
		perror("failed to open /dev/dev_ps");
		exit(1);
	}
	int bytes_read_fd0 = read(fd0, buf0, BUFSIZE);
	int bytes_read_fd1 = read(fd1, buf1, BUFSIZE);
	if(bytes_read_fd0 < 0) printf("nothing read\n");
	if(bytes_read_fd1 < 0) printf("nothing read\n");
	printf("%s\n", buf0);
	printf("%s\n", buf1);
	if(close(fd0) < 0 || close(fd1)) {
		perror("close error");
		exit(1);
	}
	free(buf0);
	free(buf1);
	return 0;
}

