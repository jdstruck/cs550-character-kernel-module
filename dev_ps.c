#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
MODULE_PARM_DESC(howmany, "Number of times to print msg");
module_param(whom, charp, S_IRUGO);
MODULE_PARM_DESC(whom, "Whom the hello is addressed to");

struct task_struct *task;
struct task_struct *task_child;
struct list_head *list;

static ssize_t my_read(struct file *file, char __user *out, size_t size, loff_t *off) {
	char *buf = "Hello World\n";
	int len = strlen(buf);
	if(size<len)
		return -EINVAL;
	if (*off != 0)
		return 0;
	if (raw_copy_to_user(out, buf, len))
		return -EINVAL;
	*off = len;
	sprintf(buf, "Hello World\n");
	return len;
}

static struct file_operations my_fops={
	.owner = THIS_MODULE,
	.read = my_read
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mymod",
	.fops = &my_fops
};

// called when module is installed
int __init init_module()
{
	int err = misc_register(&my_misc_device);
	if(err) {
		pr_err ("can't register my_misc_device");
		return err;
	}
	pr_info("I'm in!\n");
	for_each_process (task) {
		printk(KERN_ALERT "\npid: %d proc: %s cpu: %d state: %ld normal prio = %d", 
				task->pid, task->comm, task->cpu, task->state, task->normal_prio);
	}
			
	return 0;
}

// called when module is removed
void __exit cleanup_module()
{
	misc_deregister(&my_misc_device);
	int i;
	for( i=0; i<howmany; i++)
		printk(KERN_ALERT "mymodule: %d: Goodbye, cruel %s!!\n", i, whom);
}

