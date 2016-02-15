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
        printf("Usage: analysis <lat_start> <lat_end>\n");
        return -1;
    }

    char filename[256];

    int lat_start = bpf_obj_get(argv[1]);
    int lat_end = bpf_obj_get(argv[2]);
    
    struct Time {long val;};
    struct Time start = { .val = 0,};
	struct Time end = {.val = 0,};
 
    int key = 0;
    long sum = 0;
	for (key = 1; key <= 100; key++) {
       bpf_lookup_elem(lat_start, &key, &start);
       bpf_lookup_elem(lat_end, &key, &end);
       printf("%d %ld\n", key, end.val - start.val);
       sum+= (end.val - start.val);
    }

   printf("Average: %f\n", sum/(float)100);
	return 0;
}
