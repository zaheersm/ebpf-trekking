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
	if (ac != 5) {
		printf("Usage: master_host <prog_path_1> <map_path_1> <prog_path_2> <map_path_2>\n");
		return -1;
	}
	
	int prog_fd_1 = bpf_obj_get(argv[1]);
	if (prog_fd_1 < 0) {
		printf("Failed to load pinned program 1 %s\n", argv[1]);
		return -1;
	}	
	
	int map_fd_1 = bpf_obj_get(argv[2]);
	if (map_fd_1 < 0) {
		printf("Failed to load pinned map 1%s\n", argv[2]);
		return -1;
	}	
	
	int prog_fd_2 = bpf_obj_get(argv[3]);
	if (prog_fd_2 < 0) {
		printf("Failed to load to pinned program 1 %s\n", argv[3]);
		return -1;
	}
	
	int map_fd_2 = bpf_obj_get(argv[4]);
	if (map_fd_2 < 0) {
		printf("Failed to load pinned map 2 %s\n", argv[4]);
		return -1;
	}
	
	int key = 0;
	int ret = bpf_update_elem(map_fd_1, &key, &prog_fd_2, BPF_ANY);
	if (ret != 0) {
		printf("Failed to add key-value pair %s\n", strerror(errno));
		return -1;
	}

	ret = bpf_update_elem(map_fd_2, &key, &prog_fd_1, BPF_ANY);
	if (ret != 0) {
		printf("Failed to add key-value pair %s\n", strerror(errno));
		return -1;
	}

	printf("Connection successful\n");
}
