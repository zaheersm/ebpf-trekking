#include <stdio.h>
#include <assert.h>

#include <linux/bpf.h>
#include "bpf/libbpf.h"
#include "bpf/bpf_load.h"

int main (int ac, char **argv)
{
	if (ac != 4) {
		printf("Usage: persist_module <module.bpf> <prog_path> <map_path>\n");
		return -1;
	}
	
	char filename[256];
	snprintf(filename, sizeof(filename), "%s", argv[1]);
	if (load_bpf_file(filename)){
		printf("%s", bpf_log_buf);
		return -1;
	}
	
	assert(bpf_obj_pin(prog_fd[0], argv[2]) == 0);
	assert(bpf_obj_pin(map_fd[0], argv[3]) == 0);
    	
	printf("module persisted\n");
	return 0;
}
