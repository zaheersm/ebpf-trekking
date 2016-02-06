#include <uapi/linux/bpf.h>
#include "bpf/bpf_helpers.h"
#include <uapi/linux/if_ether.h>
#include <uapi/linux/ip.h>

SEC("socket1")
int main_prog (struct __sk_buff * skb)
{
    char msg [] = "In container's module\n\n";
    bpf_trace_printk(msg, sizeof(msg));
    return 0;
}
char _license[] SEC("license") = "GPL";
