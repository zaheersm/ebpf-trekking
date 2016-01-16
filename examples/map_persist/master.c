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
	
	sock = open_raw_sock("eth0");
	
	
	assert(setsockopt(sock, SOL_SOCKET, SO_ATTACH_BPF, prog_fd,
			sizeof(prog_fd[0])) == 0);

	/* Persisting map */
	assert(bpf_obj_pin(map_fd[0],"/sys/fs/bpf/pc_map") == 0);
	printf("MAP persisted to /sys/fs/bpf/pc_map\n");
	
	long icmp_cnt = 0;
	char q;
	while (q != 'q') {
		assert(bpf_lookup_elem(map_fd[0], &key, &icmp_cnt) == 0);
		printf("ICMP Count %ld | Enter 'q' to quit: ", icmp_cnt);	
		q = getchar();
	}
	
	printf("Deleting the map persisted earlier\n");
	unlink("/sys/fs/bpf/pc_map");
	sleep(1);

	return 0;
}
