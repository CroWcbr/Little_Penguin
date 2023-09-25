## Assignment 00

### Task
- Build a "Hello World module" with the following behaviour:
```
% sudo insmod main.ko
% dmesg | tail -1
[Wed May 13 12:59:18 2015] Hello world !
% sudo rmmod main.ko
% dmesg | tail -1
[Wed May 13 12:59:24 2015] Cleaning up module.
%
```

### Instraction
- main.c and Makefile

### Help
```
dmesg -T : different format
```
