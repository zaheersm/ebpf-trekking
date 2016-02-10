#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <linux/bpf.h>

#include "bpf/libbpf.h"
#include "bpf/bpf_load.h"

void print_buf (void *buf, int length)
{
	int i = 0;
	char *bp = (char *) buf;
	for (i = 0; i < length; i++)
		putchar(bp[i]);
	putchar('\n');
}

int main(int ac, char **argv) {
	
	if (ac != 3) {
		printf("Usage: demo <prog_path>\n");
		return -1;
	}
		
	int fd = bpf_obj_get(argv[1]);
	if (fd < 0) {
		printf("Failed to load pinned program %s\n", argv[1]);
		return -1;
	}

	int sock = open_raw_sock(argv[2]);
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &fd,
            sizeof(fd)) == 0);
	
	void * buffer = (void *) malloc (4096);
	while (1) {
		int length = read(sock, buffer, 4096);
		printf("Message Received L = %d\n", length);	
	}
	/*	
	FILE *f = popen("ping -c5 localhost", "r");
	(void) f; */

	return 0;
}
