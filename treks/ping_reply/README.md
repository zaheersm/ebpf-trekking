ping_block_reply.py
    - Hooks at Traffic Control (ingress queue) in Kernel using eBPF

    If packet is an ICMP echo (ping):

		* Intercepts the packet at traffic control ingress queue

		* Modifies ICMP packet type to ICMP reply

		* Swaps src/dst ip and mac and updates chksum

		* Redirects packet to the same interface as egress


