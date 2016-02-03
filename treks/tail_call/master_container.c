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

int main (int ac, char ** argv)
{
	if (ac != 3) {
		printf("Usage: master_container <container.bpf> <map_path>\n");
		return -1;
	}
	
	char filename[256];
	snprintf(filename, sizeof(filename), "%s", argv[1]);
	
	if (load_bpf_file(filename)) {
		printf("%s", bpf_log_buf);
		return -1;
	}
	
	
	assert(bpf_obj_pin(prog_fd[0], argv[2]) == 0);
	printf("container bpf module persisted to %s\n", argv[2]);
	return 0;
	

}
