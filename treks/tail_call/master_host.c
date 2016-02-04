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

int main (int ac, char **argv) 
{
	if (ac != 3) {
		printf("Usage: master_host <module.bpf> <prog_path>\n");
		return -1;
	}
	
	char filename[256];
	snprintf(filename, sizeof(filename), "%s", argv[1]);
	if (load_bpf_file(filename)){
		printf("%s", bpf_log_buf);
		return 1;
	}
	
	int key = 0;
	int fd = bpf_obj_get(argv[2]);
	if (fd < 0) {
		printf("Failed to load container module %s\n", argv[2]);
		return -1;
	}
	
	int ret = bpf_update_elem(map_fd[0], &key, &fd, BPF_ANY);
	if (ret != 0) {
		printf("Failed to add key-value pair %s\n", strerror(errno));
		return -1;
	}
	
	int sock = open_raw_sock("lo");

	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, prog_fd,
            sizeof(prog_fd[0])) == 0);

	FILE *f = popen("ping -c5 localhost", "r");
	(void) f;
	
	char q;
	while (q != 'q') {
	    printf("Enter 'q' to exit\n");
		q = getchar();
	}
		
	return 0;
}
