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
	
	int from = skb->cb[0];
	int me = 0;
	
	/*
	Determining container on the basis of  where the packet is coming from  
	*/
	if (from == 0) me = 1;
	else if (from == 1) me = 2;
	else if (from == 2) me = 3;
	else ;
	
	char msg [] = "container: %d\n";
    bpf_trace_printk(msg, sizeof(msg), me);
	
	
	skb->cb[0] = me;
	
	if (me == 1 || me == 2 || me == 3)
			bpf_tail_call(skb, &jmp_table, 0);
	
	return 0;
}
char _license[] SEC("license") = "GPL";
