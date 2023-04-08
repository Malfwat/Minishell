#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int	main(void)
{
	int	fd;

	fd = open("/dev/stdout", O_RDONLY | O_WRONLY | O_CREAT, 00644);
	
	dup2(fd, 1);
	write(1, "lol\n", 4);
	return (0);
}
