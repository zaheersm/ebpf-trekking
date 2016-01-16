ping_block_reply.py (written using iovisor/bcc, you might want to check it out)
    - Hooks at Traffic Control in Kernel using eBPF
    - If packet is an ICMP echo (ping)
        * Blocks the packet
        * Modifies it to ICMP reply
        * Swaps src/dst ip and mac and updates chksum
        * Redirects packet to the same interface as egress


