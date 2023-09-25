#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("A simple code");
MODULE_VERSION("1.0");

static __init int do_work(int my_int) {
        int x;
        int z;

        for (x = 0; x < my_int; ++x) {
                udelay(10);
        }

        if (my_int < 10) {
/*
 * That was a long sleep, tell userspace about it
 */
                pr_info("We slept a long time!");
        }

        z = x * my_int;
        return z;
}

static __init int my_init(void) {
        int x = 10;

        x = do_work(x);
        // pr_info("x = %d\n", x); // for test
        return x;
}

static __exit void my_exit(void) {}

module_init(my_init);
module_exit(my_exit);
