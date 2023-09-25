#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/time.h>
//#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("A simple hello world kernel module");
MODULE_VERSION("1.0");

/*
// my own timestamp )
static char *get_time(void)
{
        struct timespec64 ts;
        struct tm tm_info;
        char timestamp[64];

        const char *days_of_week[] = {
                "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };

        const char *months[] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        ktime_get_real_ts64(&ts);
        time64_to_tm(ts.tv_sec, 0, &tm_info);

        snprintf(timestamp, sizeof(timestamp), "[%s %s %02d %02d:%02d:%02d %04ld]",
             days_of_week[tm_info.tm_wday],
             months[tm_info.tm_mon],
             tm_info.tm_mday,
             tm_info.tm_hour,
             tm_info.tm_min,
             tm_info.tm_sec,
             tm_info.tm_year + 1900);

        return kstrdup(timestamp, GFP_KERNEL);
}
*/

static int __init hello_init(void)
{
/*
        char *timestamp = get_time();
        printk("%s 1Hello world !\n", timestamp);
// pr_info, pr_cont print timstamp.... i don't know how delete it
        kfree(timestamp);
*/
        printk(KERN_INFO "Hello world !\n");
        return 0;
}

static void __exit hello_exit(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
