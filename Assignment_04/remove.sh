#!/bin/sh

if [ -e /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko ]; then
    if /sbin/rmmod /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko; then
        echo "Module removed successfully"
    else
        echo "Failed to remove module /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko"
    fi
else
    echo "no file /home/blfs/Little_Pinguin/ex04/keyboard_dd.ko"
fi
