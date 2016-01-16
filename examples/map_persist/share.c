#include <stdio.h>
#include <linux/bpf.h>
#include <arpa/inet.h>
#include <assert.h>

#include "bpf/libbpf.h"

int main (int ac, char ** argv) 
{
	if (ac != 2) {
		printf ("Usage: share <map_path>\n");
		return -1;
	}
	
	int map_fd = bpf_obj_get(argv[1]);
	if (map_fd < 0) {
		printf("Failed to load map %s\n", argv[1]);
		return -1;
	}

	int key = IPPROTO_ICMP;
	long value = 0;
	
	assert(bpf_lookup_elem(map_fd, &key, &value) == 0);
	
	printf("ICMP count: %ld\n", value);
	return 0;
}
