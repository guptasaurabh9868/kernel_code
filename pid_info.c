#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/pid.h>
#include<linux/mm.h>
#include<linux/sched/signal.h>
#include<linux/list.h>

extern long (*address)(int *, char **);

struct task_struct *task;
int id;

void print_ancestor(struct task_struct *task)
{
	struct task_struct *temp;
	temp = task;
	int i = 1;
	while(1)
	{
		temp = temp->parent;
		printk(KERN_INFO "Parent %d :%u-%s\n",i++,temp->pid,temp->comm);
		if(temp->pid == 0) break;
	}
	
	
}

void print_sibling(struct task_struct *task)
{
	struct list_head *temp,*sib;
	struct task_struct *sibling_task;
	sib = &(task->sibling);
	int i = 0;
	list_for_each(temp,sib)
	{
		sibling_task = list_entry(temp,struct task_struct,sibling);
		printk(KERN_INFO "Sibling %d : %d-%s\n",i++,sibling_task->pid,sibling_task->comm);
	}
	
}

asmlinkage long hook_func(int *id,char **argv)
{
	printk(KERN_INFO "hook_func function is called!!!");
	//struct pid *pid_struct;
	int ID = *id;
	if(ID == 0) 
	{
		printk(KERN_INFO "Bad Pid !!");
		return -1;
	}
	else
	{
		task = pid_task(find_get_pid(ID), PIDTYPE_PID);
		print_ancestor(task);
		print_sibling(task);
		
	}
	return 0;
}

static int __init LKM_1_init(void)
{
	printk(KERN_ALERT "Module 1 Loaded\n");
	address = hook_func;
	return 0;

}

static void  __exit LKM_1_exit(void)
{
	printk(KERN_ALERT "Module 1 Unloaded\n");
	address = NULL;
}


module_init(LKM_1_init);
module_exit(LKM_1_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saurabh Gupta");
MODULE_DESCRIPTION("Process Memmory");