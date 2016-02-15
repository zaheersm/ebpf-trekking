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
	if (ac != 7) {
		printf("Usage: connect <m1> <p2> <m2> <p3> <m3> <p4>\n");
		return -1;
	}
	
	int m1 = bpf_obj_get(argv[1]);
	if (m1 < 0) {
		printf("Failed to load pinned map 1%s\n", argv[1]);
		return -1;
	}	
	
	int p2 = bpf_obj_get(argv[2]);
	if (p2 < 0) {
		printf("Failed to load to pinned program 2 %s\n", argv[2]);
		return -1;
	}
	
	int m2 = bpf_obj_get(argv[3]);
	if (m2 < 0) {
		printf("Failed to load pinned map 2 %s\n", argv[3]);
		return -1;
	}
	
	int p3 = bpf_obj_get(argv[4]);
    if (p3 < 0) {
        printf("Failed to load to pinned program 3 %s\n", argv[4]);
        return -1;
    }
    
    int m3 = bpf_obj_get(argv[5]);
    if (m3 < 0) {
        printf("Failed to load pinned map 3 %s\n", argv[5]);
        return -1;
    }

	int p4 = bpf_obj_get(argv[6]);
    if (p4 < 0) {
        printf("Failed to load to pinned program 4 %s\n", argv[6]);
        return -1;
    }
    
	/* Making connections */
	/* Left -> Right */
	int key = 0;
	int ret = bpf_update_elem(m1, &key, &p2, BPF_ANY);
	if (ret != 0) {
		printf("1 Failed to add key-value pair %s\n", strerror(errno));
		return -1;
	}

	ret = bpf_update_elem(m2, &key, &p3, BPF_ANY);
	if (ret != 0) {
		printf("2 Failed to add key-value pair %s\n", strerror(errno));
		return -1;
	}

    ret = bpf_update_elem(m3, &key, &p4, BPF_ANY);
    if (ret != 0) {
        printf("3 Failed to add key-value pair %s\n", strerror(errno));
        return -1;
    }
	
	printf("Connection successful\n");
}
