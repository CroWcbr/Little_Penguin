## Assignment 04

### Task
- Take the kernel module you wrote for task 01, and modify it so that when any USB keyboard is plugged in, the module will be automatically loaded by the correct userspace hotplug tools (which are implemented by depmod / kmod / udev / mdev / systemd, depending on what distro you are using.)

### Instraction
- i have no keyboard, so I use rules for mouse
```
my mouse identify as keyboard, i don't understend it...
so I add rules to keyboard ENV{ID_VENDOR_ID}!="046d"
```
- write rules for udev
```
98-mouse.rules
99-keyboard.rules
```

### Help
- update udev rules
```
sudo udevadm control --reload-rules
```
- add/remove usb mouse in VirtualBox for test
