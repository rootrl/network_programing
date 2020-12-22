#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <errno.h>

// see 1, https://github.com/froghui/yolanda
//     2, https://github.com/yingcheng-zhou/socket-reading-and-writing

size_t readn(int fd, void *buffer, size_t size) {

	char * buffer_pointer = buffer;

	int length = size;

	while (length > 0) {

		int result = read(fd, buffer_pointer, length);

		if (result < 0) {

			if (errno == EINTR)
				continue;

			else
				return (-1); 

		} else if (result == 0)
			break;

		length -= result;
		buffer_pointer += result;


	}

	return (size - length);

}

void read_data(int sockfd) {

	ssize_t n;

	char buf[1024];

	int time = 0;

	for (;;) {

		fprintf(stdout, "block in read\n");

		if ((n = readn(sockfd, buf, 1024)) == 0) {
			return;

		}

		time++;

		fprintf(stdout, "1K read for %d", time);

		usleep(1000);

	}


}

int main() {
	int listenfd, connfd;

	socklen_t clilen;

	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;

	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	servaddr.sin_port = htons(12345);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, 1024);

	for(;;) {

		clilen = sizeof(cliaddr);

		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, & clilen);

		read_data(connfd);

		close(connfd);
	}
}
