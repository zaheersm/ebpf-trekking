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
int main_prog (struct __sk_buff * skb)
{
	char msg [] = "In host : Tail calling container\n";
	bpf_trace_printk(msg, sizeof(msg));
	bpf_tail_call(skb, &jmp_table, 0);
	return 0;
}
char _license[] SEC("license") = "GPL";
