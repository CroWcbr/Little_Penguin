## Assignment 03

### Task
- Take the kernel module you wrote for task 01, and modify it to be create a debugfs subdirectory called "fortytwo". In that directory, create 3 virtual files called "id", "jiffies", and "foo".
- The file "id" operates just like it did for assignment 05, use the same logic there, the file must be readable and writable by any user.
- The file "jiffies" is to be read only by any user, and when read, should return the current value of the jiffies kernel timer.
- The file "foo" needs to be writable only by root, but readable by anyone. When writing to it, the value must be stored, up to one page of data. When read, which can be done by any user, the value stored in it must be returned. Properly handle the fact that someone could be reading from the file while someone else is writing to it (oh, a locking hint!)
- When the module is unloaded, all of the debugfs files are cleaned up, and any memory allocated is freed.
- Note: Besides the file’s rights, the debug directory itself need to be readable by everyone. There’s no option for that, so let’s use that old good chown !

### Instraction
- https://docs.kernel.org/filesystems/debugfs.html
- https://docs.kernel.org/locking/mutex-design.html

### Help
- test device
```
mount -t debugfs none /sys/kernel/debug

ls /sys/kernel/debug/fortytwo
```