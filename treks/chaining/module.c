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

SEC("lol")
static int main_prog (struct __sk_buff *skb)
{
	int from = skb->cb[0];
	
	int me = 0;
	if (from == 0) me = 1;
	else me = 2;	
	char msg [] = "In module of container: %d\n";
    bpf_trace_printk(msg, sizeof(msg), me);
	
    if (me == 2) {
        bpf_redirect(skb->ifindex, 1);
    }
	skb->cb[0] = me;
	bpf_tail_call(skb, &jmp_table, 0);
    return 0;
}
char _license[] SEC("license") = "GPL";
