## Assignment 02

### Task
- Take the kernel git tree from assignment 00 and change the Makefile to modify the EXTRAVERSION field. Do this in a way that the running kernel (after modifying the Makefile, rebuilding, and rebooting) has the characters "-thor_kernel" in the version string.

### Instraction
- reinstall kernel-6.5.5 whith EXTRAVERSION = -thor_kernel

### Help
- add grub info
```
mount /boot
add to grub.cfg linux-6.5.5-thor_kernel
```
- create patch
```
diff -up Makefile_6.5.5{.orig,} > thor_kernel.patch
```
