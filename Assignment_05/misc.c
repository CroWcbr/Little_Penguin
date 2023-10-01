#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/string.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("Misc driver");
MODULE_VERSION("1.0");

#define LOGIN "cdarrell"
#define LOGIN_LEN 8
static char data[LOGIN_LEN];

static int my_open(struct inode *inode, struct file *file)
{
		printk("misc device my_open : open called\n");
		printk("misc device my_open : device number %d %d\n", imajor(inode), iminor(inode));
		if (file->f_mode & FMODE_READ)
				printk("misc device my_open : open called with read permission\n");
		if (file->f_mode & FMODE_WRITE)
				printk("misc device my_open : open called with write permission\n");\
		return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
		printk("misc device my_close : close called\n");
		return 0;
}

static ssize_t my_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("misc device my_write : write called\n");
		if (LOGIN_LEN != user_len || strncmp(LOGIN, user_buffer, user_len))
		{
			printk("misc device my_write : invalid value\n");
			return -1;
		}
		int status = copy_from_user(data, user_buffer, user_len);
		if (status)
		{
				printk("misc device my_write : error during copy_from_user\n");
				return -status;
		}
		return user_len;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("misc device my_read : read called\n");
		int len = user_len < LOGIN_LEN - *ppos ? user_len : LOGIN_LEN - *ppos;
		int status = copy_to_user(user_buffer, data + *ppos, len);
		if (status)
		{
				printk("misc device my_write : error during copy_to_user\n");
				return -status;
		}
		*ppos += len;
		return len;
}

static const struct file_operations fops = {
		.owner = THIS_MODULE,
		.read = my_read,
		.write = my_write,
		.open = my_open,
		.release = my_close,
};

static struct miscdevice my_device = {
        .name = "fortytwo",
        .minor = MISC_DYNAMIC_MINOR,
        .fops = &fops,
};

/* function is called, when the module is loaded into the kernel */
static int __init my_init(void)
{
        int status;

        printk(KERN_INFO "misc device my_init : register misc device!\n");
        status = misc_register(&my_device);
		if (status)
		{
				printk("misc device my_init : error during register misc device\n");
				return -status;
		}
        return 0;
}

/* function is called, when the module is removed from the kernel */
static void __exit my_exit(void)
{
        printk(KERN_INFO "misc device my_exit : deregister misc device\n");
		misc_deregister(&my_device);
}

module_init(my_init);
module_exit(my_exit);
