## Assignment 00

### Task
- Download Linus’s latest git tree from git.kernel.org (You have to figure out which one is his. It’s not that hard, just remember what his last name is and you should be fine.)
- Build it, install it, and boot it. You can use whatever kernel configuration options you wish to use, but you must enable CONFIG_LOCALVERSION_AUTO=y.

### Instraction
- download kernel from www.kernel.org
- task like ft_linux : https://linuxfromscratch.org/lfs/view/stable/chapter10/kernel.html

### Help
- add grub info
```
mount /boot
add to grub.cfg linux-6.5.5
```
- copy file from VM to local
```
scp root@10.100.48.6:<path_VM> <path_local>
```
- log info
```
dmesg
```
