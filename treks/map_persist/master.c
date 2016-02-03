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
		printf("Usage: master <module.bpf> <map_path>\n");
		return -1;
	}
	
	char filename[256];
	
	int sock;
	snprintf(filename, sizeof(filename), "%s", argv[1]);
	if (load_bpf_file(filename)){
		printf("%s", bpf_log_buf);
		return 1;
	}
	
	/* Initializing our map with key val pair ( IPPROTO_ICMP,0) */	
	int key = IPPROTO_ICMP;
	long value = 0;
	int ret = bpf_update_elem(map_fd[0], &key, &value, BPF_ANY);
	if (ret != 0) {
		printf("Failed to add key-value pair: %s\n",strerror(errno));	
		return -1;
	}
	
	sock = open_raw_sock("lo");
	
	
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, prog_fd,
			sizeof(prog_fd[0])) == 0);

	/* Persisting map */
	assert(bpf_obj_pin(map_fd[0],argv[2]) == 0);
	printf("MAP persisted to %s\n", argv[2]);
	
	// Pinging to localhost
	FILE * f = popen("ping -c5 localhost", "r");
	(void) f; //Suppress unused variable warning from compiler

	
	long icmp_cnt = 0;
	char q;
	while (q != 'q') {
		assert(bpf_lookup_elem(map_fd[0], &key, &icmp_cnt) == 0);
		printf("ICMP Count %ld | Enter 'q' to quit: ", icmp_cnt);	
		q = getchar();
	}
	
	return 0;
}
