## Assignment 04

### Task
- Take the kernel module you wrote for task 01, and modify it so that when any USB keyboard is plugged in, the module will be automatically loaded by the correct userspace hotplug tools (which are implemented by depmod / kmod / udev / mdev / systemd, depending on what distro you are using.)

### Instraction
- write rules for udev
```
99-keyboard.rules to /etc/udev/rules.d
```

### Help
- update udev rules
```
reboot
```
