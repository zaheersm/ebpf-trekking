#include <uapi/linux/bpf.h>
#include "bpf/bpf_helpers.h"

#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>

struct bpf_map_def SEC("maps") jmp_table = {
	.type = BPF_MAP_TYPE_PROG_ARRAY,
	.key_size = sizeof(int),
	.value_size = sizeof(int),
	.max_entries = 1,
};

SEC("socket1")
static int main_prog (struct __sk_buff *skb)
{
	char msg [] = "container: %d\n";
	int me = 4;
    bpf_trace_printk(msg, sizeof(msg), 4);
	
	return -1;
}
char _license[] SEC("license") = "GPL";
