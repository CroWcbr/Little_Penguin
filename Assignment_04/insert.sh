#!/bin/sh

if [ -e /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko ]; then
    if /sbin/insmod /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko; then
        echo "Module loaded successfully"
    else
        echo "Failed to load module /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko"
    fi
else
    echo "no file /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko"
fi
