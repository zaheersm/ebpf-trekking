#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <linux/bpf.h>

#include "bpf/libbpf.h"
#include "bpf/bpf_load.h"

int main(int ac, char **argv) {

	if (ac != 2) {
		printf("Usage: demo <prog_path>\n");
		return -1;
	}
		
	int fd = bpf_obj_get(argv[1]);
	if (fd < 0) {
		printf("Failed to load pinned program %s\n", argv[1]);
		return -1;
	}

	int sock = open_raw_sock("lo");
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, &fd,
            sizeof(fd)) == 0);
	
	FILE *f = popen("ping -c5 localhost", "r");
	(void) f;
	
	char q;
	while (q != 'q') {
		printf("Enter 'q' to exit\n");
		q = getchar();
	}

	return 0;
}
