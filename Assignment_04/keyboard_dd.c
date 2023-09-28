#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("A simple keyboard kernel module");
MODULE_VERSION("1.0");

static int __init insert(void)
{
        printk(KERN_INFO "USB keyboard is plugged in\n");
        return 0;
}

static void __exit remove(void)
{
        printk(KERN_INFO "USB keyboard is plugged out\n");
}

module_init(insert);
module_exit(remove);
