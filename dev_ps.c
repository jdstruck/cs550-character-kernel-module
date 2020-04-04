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

struct task_struct *task;
struct task_struct *task_child;
struct list_head *list;

char *get_state(long state) {
	switch (state) {
		case TASK_RUNNING: return "TASK_RUNNING"; break;
		case TASK_INTERRUPTIBLE: return "TASK_INTERRUPTIBLE"; break;
		case TASK_UNINTERRUPTIBLE: return "TASK_UNINTERRUPTIBLE"; break;
		case __TASK_STOPPED: return "__TASK_STOPPED"; break;
		case __TASK_TRACED: return "__TASK_TRACED"; break;
		case EXIT_DEAD: return "EXIT_DEAD"; break;
		case EXIT_ZOMBIE: return "EXIT_ZOMBIE"; break;
		case EXIT_TRACE: return "EXIT_TRACE"; break;
		case TASK_PARKED: return "TASK_PARKED"; break;
		case TASK_DEAD: return "TASK_DEAD"; break;
		case TASK_WAKEKILL: return "TASK_WAKEKILL"; break;
		case TASK_WAKING: return "TASK_WAKING"; break;
		case TASK_NOLOAD: return "TASK_NOLOAD"; break;
		case TASK_NEW: return "TASK_NEW"; break;
		case TASK_KILLABLE: return "TASK_KILLABLE"; break;
		case TASK_STOPPED: return "TASK_STOPPED"; break;
		case TASK_TRACED: return "TASK_TRACED"; break;
		case TASK_IDLE: return "TASK_IDLE"; break;
		case TASK_NORMAL: return "TASK_NORMAL"; break;
		case TASK_REPORT: return "TASK_REPORT"; break;
		default: return "UNKNOWN STATE";
	}
}

char *proc_str(void) {
	char *pstr = kmalloc(20000, GFP_KERNEL);
	int i;
	for(i = 0; i < 20000; ++i) pstr[i] = 0;
	int counter = 0;	
	for_each_process (task) {
		char tmp[256];
		sprintf(tmp, "\nPID: %d PPID: %d CPU: %d STATE: %ld %s", 
				task->pid, task->parent->pid, task->cpu, task->state, get_state(task->state));
		strcat(pstr, tmp);
		printk(KERN_ALERT "%s", tmp);
		++counter;
	}
	//printk(KERN_ALERT "\n%d processes\n", counter);
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
}

