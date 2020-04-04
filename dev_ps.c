#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

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

char *get_state(long state) {
	switch (state) {
		case 0: return "TASK_RUNNING"; break;
		case 1: return "TASK_INTERRUPTIBLE"; break;
		case 2: return "TASK_UNINTERRUPTIBLE"; break;
		case 4: return "__TASK_STOPPED"; break;
		case 8: return "__TASK_TRACED"; break;
		case 16: return "EXIT_DEAD"; break;
		case 32: return "EXIT_ZOMBIE"; break;
		case 64: return "TASK_PARKED"; break;
		case 128: return "TASK_WAKEKILL"; break;
		case 256: return "TASK_WAKING"; break;
		case 512: return "TASK_PARKED"; break;
		case 1024: return "TASK_NOLOAD"; break;
		case 2048: return "TASK_NEW"; break;
		case 4096: return "TASK_STATE_MAX"; break;
		default: return "UNKNOWN STATE";
	}
}

char *proc_str(void) {
	char *pstr = kmalloc(20000, GFP_KERNEL);
	char *hello = "Hello From proc_str() via kmalloc DIRECT\n";
	memcpy(pstr, hello, strlen(hello));
	strcat(pstr, "This is another line\n");
	int counter = 0;	
	for_each_process (task) {
		char tmp[256];
		sprintf(tmp, "\nPID: %d PPID: %d CPU: %d STATE: %ld", 
				task->pid, task->parent->pid, task->cpu, task->state);
		strcat(pstr, tmp);
		printk(KERN_ALERT "%s", tmp);
		++counter;
	}
	printk(KERN_ALERT "\n%d processes\n", counter);
	
	kfree(pstr);
	return pstr;
}

static ssize_t my_read(struct file *file, char __user *out, size_t size, loff_t *off) {
	char *pstr = proc_str();
	int len = strlen(pstr);

	if(size<len)
		return -EINVAL;
	if (*off != 0)
		return 0;
	if (copy_to_user(out, pstr, len))
		return -EINVAL;
	return len;
}

static struct file_operations my_fops={
	.owner = THIS_MODULE,
	.read = my_read
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "dev_ps",
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

