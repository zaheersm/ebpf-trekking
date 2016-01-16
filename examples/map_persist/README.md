#####Overview

This example is based on [BPF samples](https://github.com/torvalds/linux/tree/master/samples/bpf) in linux tree.

A BPF module (module.c) counts the ICMP ping packets. Count is stored in a BPF 
map. A user space program (master.c) loads the BPF module, attaches it and 
pins the map. A separate user-space program (share.c) reads from the pinned map 
and removes/unpins the map.

**module.c**: A BPF program written in restricted C. It checks if the packet is 
an ICMP ping. If yes, it increments a counter. Counter is a bpf map of type 
BPF_MAP_TYPE_HASH.

**module.bpf**: An executable BPF program (module.c gets compiled into module.bpf 
using LLVM's bpf backend). You need to pass module.bpf to master.c

**master.c**: A user space program which loads a bpf module, attaches it and pins 
the map using bpf_obj_pin to /sys/fs/bpf/pc_map

**share.c**: Another user space program which reads a value from the map pinned 
by master.c. You need to pass /sys/fs/bpf/pc_map as argument to this program. 
Before exiting, it unpins/removes the map.

#####Getting it up and running
Make sure /sys/fs/bpf is mounted. This is where our map will be persisted.
```bash
mount | grep bpf
```
If you don't get any output, mount /sys/fs/bpf.
```bash
sudo mount -t bpf none /sys/fs/bpf
```
Compile restricted C code to bpf byte code using LLVM backend
(If you don't have LLVM setup, you could use module.bpf already present in this directory)
```bash
clang -O2 -target bpf -c module.c -o module.bpf
```
Compile master.c and share.c
```bash
gcc bpf/libbpf.c bpf/bpf_load.c -l elf master.c -o master
gcc bpf/libbpf.c share.c -o share
```
Executing master
```bash
sudo ./master module.bpf
```
Executing share (you can execute it even after quiting master)
```bash
sudo ./share /sys/fs/bpf/pc_map
```
You need linux kernel version >= 4.4 to run this example. It make use of Kernel 
headers which might not have been exported to use by user-space if you've updated 
your Kernel. You can export your kernel headers by going through the [guide](https://www.kernel.org/doc/Documentation/kbuild/headers_install.txt).

You can setup LLVM backend for ebpf by using the [script](https://gist.github.com/tuxology/357d8826e97eb72c9277) provided by [Suchakra Sharma](https://suchakra.wordpress.com/about/).
