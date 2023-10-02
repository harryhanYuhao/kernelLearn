# Kernel Developping

## Kernel Modules

Kernel Modules is anything that can be loaded and unloaded from the kernel.

Useful commmands:

- `insmod <module-name>`: load the module 
- `rmmod <module-name>`: remove the module
- `lsmod`: show all modules 
- `modinfo <module-name>`: show module information
- `journalctl --since "x minute ago" | grep kernel`: 
- `strace <path-to-file>`: trace syscalls


## Build Modules

To build modules from c file named `hello-1.c`, consider the following makefile:

```makefile
obj-m += hello-1.o 
obj-m += hello-2.o

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	EXTRA_CFLAGS='-save-temps'

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

EXTRA_CFLAGS='-save-temps'  # save the temporary files
```

To build the module, you may need to switch to root user with `sudo -Es`. `-E` flag is to preserve the bash variables.

### Compilation and Linking: some insights

The kernel module shall only invoke the symbols that are contained in the kernel (can be found in `/proc/kallsyms`. C library is not part of kernel. 

In the module file we invoked the function `pr_info()` without linking. 
This is because module are merely object files whose symbol was resolved when it is inserted into the kernel by `insmod`.

### Namespace pollution

To avoid namespace pollution:

- use static variable
- prefix each variable with lower case letter (kernel convention)
- register the variable in the symbol table.

### Memory

Each module shares the kernel's codespace memory. So module's segfault is also the kernel's. Most monolithic kernel behaves like this. There are also microkenrels like GNU Hurd and Zircon whose module may be in independent codespace.

### Device drivers

On unix, each hardware is represented by a file located in `/dev`. So a device driver may be linked to `/dev/sound`, and a user space program can access `/dev/sound` without knowing the driver.

The output of `lsblk` is:

```
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
nvme0n1     259:0    0 238.5G  0 disk 
├─nvme0n1p1 259:1    0     4G  0 part /boot
├─nvme0n1p2 259:2    0     8G  0 part [SWAP]
└─nvme0n1p3 259:3    0 226.5G  0 part /
```

The output of `ls \dev\` is something like:

```
crw-------  10,123 root 30 Sep 18:03 acpi_thermal_rel
crw-r--r--  10,235 root 30 Sep 18:03 autofs
drwxr-xr-x       - root 30 Sep 18:03 block
crw-------  10,234 root 30 Sep 18:03 btrfs-control
drwxr-xr-x       - root 30 Sep 18:03 bus
drwxr-xr-x       - root 30 Sep 18:03 char
crw-------     5,1 root 30 Sep 18:03 console
lrwxrwxrwx    141T root 30 Sep 18:03 core -> /proc/kcore
drwxr-xr-x       - root 30 Sep 18:03 cpu
crw-------  10,124 root 30 Sep 18:03 cpu_dma_latency
crw-------  10,203 root 30 Sep 18:03 cuse
drwxr-xr-x       - root 30 Sep 18:03 disk
drwxr-xr-x       - root 30 Sep 18:03 dma_heap
drwxr-xr-x       - root 30 Sep 18:03 dri
crw-------   238,0 root 30 Sep 18:03 drm_dp_aux0
crw-rw----    29,0 root 30 Sep 18:03 fb0
lrwxrwxrwx       - root 30 Sep 18:03 fd -> /proc/self/fd
crw-rw-rw-     1,7 root 30 Sep 18:03 full
crw-rw-rw-     1,3 root 30 Sep 18:03 null
crw-------   237,0 root 30 Sep 18:03 nvme0
brw-rw----     3,0 root 30 Sep 18:03 nvme0n1
brw-rw----     3,1 root 30 Sep 18:03 nvme0n1p1
brw-rw----     3,2 root 30 Sep 18:03 nvme0n1p2
brw-rw----     3,3 root 30 Sep 18:03 nvme0n1p3
crw-------  10,144 root 30 Sep 18:03 nvram
crw-r-----     1,4 root 30 Sep 18:03 port
crw-------   108,0 root 30 Sep 18:03 ppp
crw-------    10,1 root 30 Sep 18:03 psaux
crw-rw-rw-     5,2 root 30 Sep 19:04 ptmx
drwxr-xr-x       - root 30 Sep 18:03 pts
crw-rw-rw-     1,8 root 30 Sep 18:03 random
crw-rw-r--@ 10,242 root 30 Sep 18:03 rfkill
lrwxrwxrwx   248,0 root 30 Sep 18:03 rtc -> rtc0
crw-------   248,0 root 30 Sep 18:03 rtc0
drwxrwxrwt       - root 30 Sep 18:03 shm
crw-------  10,231 root 30 Sep 18:03 snapshot
drwxr-xr-x       - root 30 Sep 18:03 snd
lrwxrwxrwx   136,0 root 30 Sep 18:03 stderr -> /proc/self/fd/2
lrwxrwxrwx   136,0 root 30 Sep 18:03 stdin -> /proc/self/fd/0
lrwxrwxrwx       0 root 30 Sep 18:03 stdout -> /proc/self/fd/1
crw-rw----  10,224 tss  30 Sep 18:03 tpm0
crw-rw----   253,0 root 30 Sep 18:03 tpmrm0
crw-rw-rw-     5,0 root 30 Sep 18:32 tty
crw--w----     4,0 root 30 Sep 18:03 tty0
crw-------     4,1 hhyh 30 Sep 18:56 tty1
crw--w----     4,2 root 30 Sep 18:03 tty2
crw--w----     4,3 root 30 Sep 18:03 tty3
crw--w----     4,4 root 30 Sep 18:03 tty4
crw--w----     4,5 root 30 Sep 18:03 tty5
crw--w----     4,6 root 30 Sep 18:03 tty6
crw--w----     4,7 root 30 Sep 18:03 tty7
crw--w----     4,8 root 30 Sep 18:03 tty8
crw--w----     4,9 root 30 Sep 18:03 tty9
crw--w----    4,10 root 30 Sep 18:03 tty10
crw--w----    4,11 root 30 Sep 18:03 tty11
crw--w----    4,12 root 30 Sep 18:03 tty12
crw--w----    4,13 root 30 Sep 18:03 tty13
crw--w----    4,14 root 30 Sep 18:03 tty14
crw--w----    4,15 root 30 Sep 18:03 tty15
...
rw-rw----    4,87 root 30 Sep 18:03 ttyS23
crw-rw----    4,88 root 30 Sep 18:03 ttyS24
crw-rw----    4,89 root 30 Sep 18:03 ttyS25
crw-rw----    4,90 root 30 Sep 18:03 ttyS26
crw-rw----    4,91 root 30 Sep 18:03 ttyS27
crw-rw----    4,92 root 30 Sep 18:03 ttyS28
```

The number separated by comma is the device major number and minor number. Each driver is assigned to a unique major number, which is shown here before the comma, denoting the associated driver. The minor number is merely for the driver to distinguish the various hardware. 

