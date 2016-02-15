#include <uapi/linux/bpf.h>
#include "bpf/bpf_helpers.h"

#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>

struct Time { long val;};
struct bpf_map_def SEC("maps") jmp_table = {
    .type = BPF_MAP_TYPE_PROG_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(int),
    .max_entries = 1,
};

struct bpf_map_def SEC("maps") latency = {
    .type = BPF_MAP_TYPE_HASH,
    .key_size = sizeof(int),
    .value_size = sizeof(struct Time),
    .max_entries = 100,
};

struct bpf_map_def SEC("maps") counter = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(int),
    .max_entries = 1,
};

SEC("socket1")
static int main_prog (struct __sk_buff *skb)
{
    int from = skb->cb[0];
    int me = 0;
    if (from == 3)
        me = 4;
    
    int init_val = 0;
    
    int key = 0;
    int *count = bpf_map_lookup_elem(&counter, &key);
    if (count)
        __sync_fetch_and_add(count, 1);
    else {
        bpf_map_update_elem(&counter, &key, &init_val, BPF_ANY);
        char msg [] = "container: %d | count %d \n";
        bpf_trace_printk(msg, sizeof(msg), me, 0);
        return 0;
    }
    
    struct Time tm = {
        .val = bpf_ktime_get_ns(),
    };

    int seq = 0;
    if (count) {
        seq = *count;
        if (seq >= 101)
            return 0;
        char msg [] = "container: %d | count %d \n";
        bpf_trace_printk(msg, sizeof(msg), me, seq);
        struct Time tm = { .val = bpf_ktime_get_ns(),};
        bpf_map_update_elem(&latency, &seq, &tm, BPF_ANY);
    }
    return 0;
}
char _license[] SEC("license") = "GPL";
