#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/signal.h>
#include <linux/sched.h>

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

// called when module is installed
int __init init_module()
{
	for_each_process (task) {
		printk(KERN_ALERT "\npid: %d proc: %s cpu: %d state: %ld normal prio = %d", task->pid, task->comm, task->cpu, task->state, task->normal_prio);
	}
			
	return 0;
}



// called when module is removed
void __exit cleanup_module()
{
	int i;

	for( i=0; i<howmany; i++)
		printk(KERN_ALERT "mymodule: %d: Goodbye, cruel %s!!\n", i, whom);
}

