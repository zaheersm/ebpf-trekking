#include <stddef.h>
#include <uapi/linux/bpf.h>
#include <uapi/linux/if_ether.h>
#include <uapi/linux/if_packet.h>
#include <uapi/linux/ip.h>
#include <uapi/linux/icmp.h>
	
#include "bpf/bpf_helpers.h"

struct bpf_map_def SEC("maps") pc_map = {
	.type = BPF_MAP_TYPE_HASH,
	.key_size = sizeof(int),
	.value_size = sizeof(long),
	.max_entries = 1,
};

/* 
Using bpf_load.c, we need to pass SEC("socket1") for 
BPF_PROG_TYPE_SOCKET_FILTER
*/

/* Assuming ICMP packets we receive are not fragmented */
#define IP_HLEN 20

SEC("socket1")
int pc_prog (struct __sk_buff * skb) 
{
	int proto = load_byte (skb, ETH_HLEN + offsetof(struct iphdr, protocol));
	
	if (proto != 1)
		return 0;

	int type = load_byte (skb, ETH_HLEN + IP_HLEN + 
								offsetof(struct icmphdr, type));
	
	int code = load_byte (skb, ETH_HLEN + IP_HLEN + 
								offsetof(struct icmphdr, code));
	
	
	if (type != 8 && code != 0)
		return 0;
	
	/*
	char msg [] = "proto: %d | type %d | code %d\n";
	bpf_trace_printk(msg, sizeof(msg), proto, type, code);
	*/

	char msg [] = "ICMP Ping received\n";
	bpf_trace_printk(msg, sizeof(msg));

	
	long *count = bpf_map_lookup_elem(&pc_map, &proto);
	if (count)
		__sync_fetch_and_add(count, 1);

	return 0;
}
char _license[] SEC("license") = "GPL";
