// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdarrell");
MODULE_DESCRIPTION("Mount info module");

#define MY_PROC_NAME "mymount"

static struct proc_dir_entry *proc_file;

static int my_proc_show(struct seq_file *m, void *v)
{
	struct mount *mnt;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		if (strcmp(mnt->mnt_devname, "rootfs")) {
			struct path mnt_path = { .dentry = mnt->mnt.mnt_root, .mnt = &mnt->mnt };

			seq_printf(m, "%-15s", mnt->mnt_devname);
			seq_path(m, &mnt_path, " \t\n\\");
			seq_putc(m, '\n');
		}
	}
	return 0;
}

static int my_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, my_proc_show, NULL);
}

static const struct proc_ops my_proc_fops = {
	.proc_open = my_proc_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init my_mount_info_init(void)
{
	proc_file = proc_create(MY_PROC_NAME, 0, NULL, &my_proc_fops);
	if (!proc_file) {
		pr_err("%s: failed to create /proc/%s file\n", __func__, MY_PROC_NAME);
		return -ENOMEM;
	}
	pr_info("/proc/%s created\n", MY_PROC_NAME);
	return 0;
}

static void __exit my_mount_info_exit(void)
{
	proc_remove(proc_file);
	pr_info("/proc/%s removed\n", MY_PROC_NAME);
}

module_init(my_mount_info_init);
module_exit(my_mount_info_exit);
