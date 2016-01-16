# To compile restricted C code to bpf byte code using LLVM backend
# (If you don't have LLVM setup, you could use module.bpf already present in this directory)
clang -O2 -target bpf -c module.c -o module.bpf

# To compile master.c and share.c
gcc bpf/libbpf.c bpf/bpf_load.c -l elf master.c -o master
gcc bpf/libbpf.c share.c -o share

# Executing master
sudo ./master module.bpf

#Executing share (without quitting master)
sudo ./share /sys/fs/bpf/pc_map

