#include <uapi/linux/bpf.h>
#include "bpf/bpf_helpers.h"

#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>

struct bpf_map_def SEC("maps") jmp_table = {
	.type = BPF_MAP_TYPE_PROG_ARRAY,
	.key_size = sizeof(int),
	.value_size = sizeof(int),
	.max_entries = 2,
};

SEC("socket1")
static int main_prog (struct __sk_buff *skb)
{
	
	/* 
	dir == 0 : left -> right
	dir == 1 : right -> left
	*/
	int dir = skb->cb[0];
	int from = skb->cb[1];
	int me = 0;
	
	/*
		Determining container on the basis of  where the packet is coming from and 
		what is the direction
	*/
	if (dir == 0) { // Going left -> right
		if (from == 0) me = 1;
		else if (from == 1) me = 2;
		else if (from == 2) me = 3;
		else me = 4;
	} else if (dir == 1) { // Going right -> left
		if (from == 4) me = 3;
		else if (from == 3) me = 2;
		else if (from == 2) me = 1;
		else ;
	}	
	else return 0;
	
	char msg [] = "container: %d dir: %d\n";
    bpf_trace_printk(msg, sizeof(msg), me, dir);
	
	
	skb->cb[1] = me;

	if (me == 4) { // Reached end | let's go back
		skb->cb[0] = 1;	
		bpf_tail_call(skb, &jmp_table, 1);
	}
	
	if (dir == 0)
		if (me == 1 || me == 2 || me == 3)
			bpf_tail_call(skb, &jmp_table, 0);
	if (dir == 1)
		if (me == 3 || me == 2)
			bpf_tail_call(skb, &jmp_table, 1);
	
	return 0;
}
char _license[] SEC("license") = "GPL";
