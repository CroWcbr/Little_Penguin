#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>

#include <linux/jiffies.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("debugfs");
MODULE_VERSION("1.0");

#define LOGIN "cdarrell"
#define LOGIN_LEN 8
static char data_id[LOGIN_LEN];

static struct mutex foo_mutex;
static char data_foo[PAGE_SIZE];
static size_t data_foo_len = 0;

static struct dentry *dir = NULL;

static int my_open(struct inode *inode, struct file *file)
{
		printk("my_open : open called %s\n", file->f_path.dentry->d_name.name);
		if (file->f_mode & FMODE_READ)
				printk("my_open : open called with read permission : %s\n", file->f_path.dentry->d_name.name);
		if (file->f_mode & FMODE_WRITE)
				printk("my_open : open called with write permission : %s\n", file->f_path.dentry->d_name.name);
		return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
		printk("my_close : close called %s\n", file->f_path.dentry->d_name.name);
		return 0;
}

static ssize_t id_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("id_write : write called\n");
		
		if (LOGIN_LEN != user_len || strncmp(LOGIN, user_buffer, user_len))
		{
			printk("id_write : invalid value\n");
			return -1;
		}
		int status = copy_from_user(data_id, user_buffer, user_len);
		if (status)
		{
				printk("id_write : error during copy_from_user\n");
				return -status;
		}
		return user_len;
}

static ssize_t id_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("id_read : read called\n");
		int len = user_len < LOGIN_LEN - *ppos ? user_len : LOGIN_LEN - *ppos;
		int status = copy_to_user(user_buffer, data_id + *ppos, len);
		if (status)
		{
				printk("id_read : error during copy_to_user\n");
				return -status;
		}
		*ppos += len;
		return len;
}

static ssize_t foo_write(struct file *file, const char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("foo_write : write called\n");
		mutex_lock(&foo_mutex);
        if (user_len > PAGE_SIZE)
		{
				mutex_unlock(&foo_mutex);
				return ( -EINVAL);
		}
		int status = simple_write_to_buffer(data_foo, sizeof(data_foo), ppos, user_buffer, user_len);
		if (status >= 0)
				data_foo_len = status;
		mutex_unlock(&foo_mutex);
		if (status < 0)
		{
				printk("foo_read : error during simple_write_to_buffer\n");
				return (-status);
		}
		return status;
}

static ssize_t foo_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
		printk("foo_read : read called\n");
		mutex_lock(&foo_mutex);
		int status = simple_read_from_buffer(user_buffer, user_len, ppos, data_foo, data_foo_len);
		mutex_unlock(&foo_mutex);
		if (status < 0)
		{
				printk("foo_read : error during simple_read_from_buffer\n");
				return (-status);
		}
		return status;
}

static struct file_operations id_fops = {
		.owner = THIS_MODULE,
		.open = my_open,
		.release = my_close,
		.read = id_read,
		.write = id_write,
};

static struct file_operations foo_fops = {
		.owner = THIS_MODULE,
		.open = my_open,
		.release = my_close,
		.read = foo_read,
		.write = foo_write,
};

static int debugfs_module_check(struct dentry *file, char *filename)
{
		if (IS_ERR_OR_NULL(file))
		{
				int status = (file ? PTR_ERR(file) : -ENOMEM);
				pr_err("my_init : Failed to create %s\n", filename);
				debugfs_remove_recursive(dir);
				pr_err("my_init : debugfs_remove_recursive\n");
				return status;
		}
		return 0;
}

/* function is called, when the module is loaded into the kernel */
static int __init my_init(void)
{
		printk(KERN_INFO "my_init : register\n");
		int status = 0;

		dir = debugfs_create_dir("fortytwo", NULL);
		if ((status = debugfs_module_check(dir, "fortytwo")))
			return status;
		printk(KERN_INFO "my_init : fortytwo dir created.\n");

		struct dentry *file = NULL;

		file = debugfs_create_file("id", 0666, dir, NULL, &id_fops);
		if ((status = debugfs_module_check(file, "id")))
			return status;
		printk(KERN_INFO "my_init : id file created.\n");

		debugfs_create_u64("jiffies", 0444, dir, (u64 *)&jiffies);
		printk(KERN_INFO "my_init : jiffies file created.\n");

		file = debugfs_create_file("foo", 0644, dir, NULL, &foo_fops);
		if ((status = debugfs_module_check(file, "foo")))
			return status;
		printk(KERN_INFO "my_init : foo file created.\n");

		printk(KERN_INFO "my_init : register successfully\n");
		return (0);
}

/* function is called, when the module is removed from the kernel */
static void __exit my_exit(void)
{
		debugfs_remove_recursive(dir);
		printk(KERN_INFO "my_exit : deregister\n");
}

module_init(my_init);
module_exit(my_exit);
