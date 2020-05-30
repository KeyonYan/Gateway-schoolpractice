#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CHAR_DEV_NAME "/dev/sensor_dev"
 
int main()
{
	int ret;
	int fd;
	char buf[32];
 
	fd = open(CHAR_DEV_NAME, O_RDONLY | O_NDELAY);
	if(fd < 0)
	{
		printf("open failed!\n");
		return -1;
	}

    read(fd, buf, 2);
    printf("buf[0]: %d\n", buf[0]);
    printf("buf[1]: %d\n", buf[1]);

	close(fd);
	
	return 0;
}