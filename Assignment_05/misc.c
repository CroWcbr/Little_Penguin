// SPDX-License-Identifier: GPL-2.0
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
	pr_info("misc device %s : open called\n", __func__);
	pr_info("misc device %s : device number %d %d\n", __func__, imajor(inode), iminor(inode));
	if (file->f_mode & FMODE_READ)
		pr_info("misc device %s : open called with read permission\n", __func__);
	if (file->f_mode & FMODE_WRITE)
		pr_info("misc device %s : open called with write permission\n", __func__);
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	pr_info("misc device %s : close called\n", __func__);
	return 0;
}

static ssize_t my_write(struct file *file, const char __user *u_buf, size_t user_len, loff_t *ppos)
{
	pr_info("misc device %s : write called\n", __func__);
	if (user_len != LOGIN_LEN || strncmp(u_buf, LOGIN, user_len)) {
		pr_err("misc device %s : invalid value\n", __func__);
		return -1;
	}
	int status = copy_from_user(data, u_buf, user_len);

	if (status) {
		pr_err("misc device %s : error during copy_from_user\n", __func__);
		return -status;
	}
	return user_len;
}

static ssize_t my_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	pr_info("misc device %s : read called\n", __func__);
	int len = user_len < LOGIN_LEN - *ppos ? user_len : LOGIN_LEN - *ppos;
	int status = copy_to_user(user_buffer, data + *ppos, len);

	if (status) {
		pr_err("misc device %s : error during copy_to_user\n", __func__);
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

	pr_info("misc device %s : register misc device!\n", __func__);
	status = misc_register(&my_device);
	if (status) {
		pr_err("misc device %s : error during register misc device\n", __func__);
		return -status;
	}
	return 0;
}

/* function is called, when the module is removed from the kernel */
static void __exit my_exit(void)
{
	pr_info("misc device %s: deregister misc device\n", __func__);
	misc_deregister(&my_device);
}

module_init(my_init);
module_exit(my_exit);
