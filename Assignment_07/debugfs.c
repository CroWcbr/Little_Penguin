// SPDX-License-Identifier: GPL-2.0
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
static size_t data_foo_len;

static struct dentry *dir;

static int my_open(struct inode *inode, struct file *file)
{
	const char *file_name = file->f_path.dentry->d_name.name;

	pr_info("%s : open called %s\n", __func__, file_name);
	if (file->f_mode & FMODE_READ)
		pr_info("%s : open called with read permission : %s\n", __func__, file_name);
	if (file->f_mode & FMODE_WRITE)
		pr_info("%s : open called with write permission : %s\n", __func__, file_name);
	return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
	pr_info("%s : close called %s\n", __func__, file->f_path.dentry->d_name.name);
	return 0;
}

static ssize_t id_write(struct file *file, const char __user *u_buffer, size_t u_len, loff_t *ppos)
{
	pr_info("%s : write called\n", __func__);

	if (u_len != LOGIN_LEN || strncmp(u_buffer, LOGIN, u_len)) {
		pr_err("%s : invalid value\n", __func__);
		return -1;
	}
	int status = copy_from_user(data_id, u_buffer, u_len);

	if (status) {
		pr_err("%s : error during copy_from_user\n", __func__);
		return -status;
	}
	return u_len;
}

static ssize_t id_read(struct file *file, char __user *u_buffer, size_t u_len, loff_t *ppos)
{
	pr_info("%s : read called\n", __func__);
	int len = u_len < LOGIN_LEN - *ppos ? u_len : LOGIN_LEN - *ppos;
	int status = copy_to_user(u_buffer, data_id + *ppos, len);

	if (status) {
		pr_err("%s : error during copy_to_user\n", __func__);
		return -status;
	}
	*ppos += len;
	return len;
}

static ssize_t foo_write(struct file *file, const char __user *u_buffer, size_t u_len, loff_t *ppos)
{
	pr_info("%s : write called\n", __func__);
	loff_t  tmp;
	int status;

	if (file->f_flags & O_APPEND)
		tmp = data_foo_len;
	else
		tmp = 0;
	loff_t pos = *ppos + tmp;
	// printk("foo_write : tmp =  %lld\n", tmp);

	if (u_len + tmp > PAGE_SIZE)
		return (-EINVAL);
	mutex_lock(&foo_mutex);
	status = simple_write_to_buffer(data_foo, sizeof(data_foo), &pos, u_buffer, u_len);

	mutex_unlock(&foo_mutex);
	if (status > 0) {
		data_foo_len = pos;
		data_foo[data_foo_len] = '\0';
	}
	if (status < 0) {
		pr_err("%s : error during simple_write_to_buffer\n", __func__);
		return (-status);
	}
	*ppos = pos - tmp;
	return status;
}

static ssize_t foo_read(struct file *file, char __user *user_buffer, size_t user_len, loff_t *ppos)
{
	pr_info("%s : read called\n", __func__);

	if (*ppos >= data_foo_len)
		return 0;
	mutex_lock(&foo_mutex);
	int status = simple_read_from_buffer(user_buffer, user_len, ppos, data_foo, data_foo_len);

	mutex_unlock(&foo_mutex);
	if (status < 0) {
		pr_err("%s : error during simple_read_from_buffer\n", __func__);
		return (-status);
	}
	*ppos += status;
	return status;
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = id_read,
	.write = id_write,
};

static const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = foo_read,
	.write = foo_write,
};

static int debugfs_module_check(struct dentry *file, char *filename)
{
	if (IS_ERR_OR_NULL(file)) {
		int status = (file ? PTR_ERR(file) : -ENOMEM);

		pr_err("my_init : Failed to create %s\n", filename);
		debugfs_remove_recursive(dir);
		pr_info("my_init : debugfs_remove_recursive\n");
		return status;
	}
	return 0;
}

/* function is called, when the module is loaded into the kernel */
static int __init my_init(void)
{
	pr_info("%s : register\n", __func__);
	int status = 0;

	dir = debugfs_create_dir("fortytwo", NULL);
	status = debugfs_module_check(dir, "fortytwo");
	if (status)
		return status;
	pr_info("%s : fortytwo dir created.\n", __func__);

	struct dentry *file = NULL;

	file = debugfs_create_file("id", 0666, dir, NULL, &id_fops);
	status = debugfs_module_check(file, "id");
	if (status)
		return status;
	pr_info("%s : id file created.\n", __func__);

	debugfs_create_u64("jiffies", 0444, dir, (u64 *)&jiffies);
	pr_info("%s : jiffies file created.\n", __func__);

	file = debugfs_create_file("foo", 0644, dir, NULL, &foo_fops);
	status = debugfs_module_check(file, "foo");
	if (status)
		return status;
	pr_info("%s : foo file created.\n", __func__);

	pr_info("%s : register successfully\n", __func__);
	return 0;
}

/* function is called, when the module is removed from the kernel */
static void __exit my_exit(void)
{
	debugfs_remove_recursive(dir);
	pr_info("%s : deregister\n", __func__);
}

module_init(my_init);
module_exit(my_exit);
