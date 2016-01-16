#####Overview

This example is based on samples provided here: https://github.com/torvalds/linux/tree/master/samples/bpf.

A BPF module (module.c) counts the ICMP ping packets. Count is stored in a BPF map. A user space  
program (master.c) loads the BPF module and attaches it and pins the map. A separate user-space program 
reads from 
the pinned map.

module.c: A BPF program written in restricted C. It checks if the packet is an ICMP ping. If yes, it 
increments a counter. Counter is a bpf map of type BPF_MAP_TYPE_HASH.

module.bpf: An executable BPF program (module.c gets compiled into module.bpf 
using LLVM bpf backend). You need to pass module.bpf to master.c

master.c: A user space program which loads a bpf module, attaches it and pins the 
map using bpf_obj_pin to /sys/fs/bpf/pc_map

share.c: Another user space program which reads a value from the map pinned 
by master.c. You need to pass /sys/fs/bpf/pc_map as argument to this program.
 
#####Getting it up and running
Make sure /sys/fs/bpf is mounted. This where our map will be persisted.
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
Executing share (without quitting master)
```bash
sudo ./share /sys/fs/bpf/pc_map
```
