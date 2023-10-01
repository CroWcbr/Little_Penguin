## Assignment 05

### Task
- Take the kernel module you wrote for task 01, and modify it to be a misc char device driver. The misc interface is a very simple way to be able to create a character device, without having to worry about all of the sysfs and character device registration mess. And what a mess it is, so stick to the simple interfaces wherever possible.
- The misc device should be created with a dynamic minor number, no need to run off and trying to reserve a real minor number for your test module, that would be crazy.
- The misc device should implement the read and write functions.
- The misc device node should show up in /dev/fortytwo.
- When the character device node is read from, your student login is returned to the caller.
- When the character device node is written to, the data sent to the kernel needs to be checked. If it matches your assigned student login, then return a correct write return value. If the value does not match your assigned student login, return the "invalid value" error value.
- The misc device should be registered when your module is loaded, and unregistered when it is unloaded.

### Instraction
- book : Linux Device Driver 3rd Edition

### Help
- test device
```
root [ /home/blfs/Little_Pinguin/ex05 ]# make
make -C /lib/modules/6.5.5-thor_kernel/build M=/home/blfs/Little_Pinguin/ex05 modules
make[1]: Entering directory '/usr/src/kernel-6.5.5-thor_kernel'
  CC [M]  /home/blfs/Little_Pinguin/ex05/misc.o
  MODPOST /home/blfs/Little_Pinguin/ex05/Module.symvers
  CC [M]  /home/blfs/Little_Pinguin/ex05/misc.mod.o
  LD [M]  /home/blfs/Little_Pinguin/ex05/misc.ko
make[1]: Leaving directory '/usr/src/kernel-6.5.5-thor_kernel'
root [ /home/blfs/Little_Pinguin/ex05 ]# lsmod
Module                  Size  Used by
root [ /home/blfs/Little_Pinguin/ex05 ]# ls /dev/fortytwo
ls: cannot access '/dev/fortytwo': No such file or directory
root [ /home/blfs/Little_Pinguin/ex05 ]# insmod misc.ko
root [ /home/blfs/Little_Pinguin/ex05 ]# ls /dev/fortytwo
/dev/fortytwo
root [ /home/blfs/Little_Pinguin/ex05 ]# ls /dev/fortytwo -la
crw------- 1 root root 10, 125 Oct  1 22:15 /dev/fortytwo
root [ /home/blfs/Little_Pinguin/ex05 ]# echo -n "cdarrellasdasda" > /dev/fortytwo
-bash: echo: write error: Operation not permitted
root [ /home/blfs/Little_Pinguin/ex05 ]# echo -n "cdarr" > /dev/fortytwo
-bash: echo: write error: Operation not permitted
root [ /home/blfs/Little_Pinguin/ex05 ]# cat /dev/fortytwo
root [ /home/blfs/Little_Pinguin/ex05 ]# echo -n "cdarrell" > /dev/fortytwo
root [ /home/blfs/Little_Pinguin/ex05 ]# cat /dev/fortytwo
cdarrellroot [ /home/blfs/Little_Pinguin/ex05 ]#rmmod misc.ko
root [ /home/blfs/Little_Pinguin/ex05 ]# ls /dev/fortytwo
ls: cannot access '/dev/fortytwo': No such file or directory
root [ /home/blfs/Little_Pinguin/ex05 ]# lsmod
Module                  Size  Used by
```