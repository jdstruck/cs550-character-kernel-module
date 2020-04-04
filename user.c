#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 20000

int main() {
	char *buf = malloc(BUFSIZE);
	int fscanf_output = 0;
	int fd = open("/dev/dev_ps", O_RDWR);
	if (fd < 0) {
		perror("failed to open /dev/dev_ps");
		exit(1);
	}
	int bytes_read = read(fd, buf, BUFSIZE);
	if(bytes_read < 0) printf("nothing read\n");
	printf("\n%s\nbytes read: %d\n", buf, bytes_read);
	if(close(fd) < 0) {
		perror("close error");
		exit(1);
	}
	free(buf);
	return 0;
}

