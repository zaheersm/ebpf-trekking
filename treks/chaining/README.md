1. sudo mkdir /bpf

2. sudo mount -t bpf none /bpf

3. clang -O2 -target bpf -c module.c -o module.bpf

4. gcc bpf/libbpf.c bpf/bpf_load.c -l elf persist_module.c -o pm

5. gcc bpf/libbpf.c bpf/bpf_load.c -l elf connect.c -o c

6. gcc bpf/libbpf.c bpf/bpf_load.c -l elf demo.c -o d

7. 
sudo ./pm module.bpf /bpf/p1 /bpf/m1; 
sudo ./pm module.bpf /bpf/p2 /bpf/m2; 
sudo ./pm module.bpf /bpf/p3 /bpf/m3; 
sudo ./pm module.bpf /bpf/p4 /bpf/m4

8. sudo ./c /bpf/p1 /bpf/m1 /bpf/p2 /bpf/m2 /bpf/p3 /bpf/m3 /bpf/p4 /bpf/m4

9. sudo ./d /bpf/p1
