#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include<asm/uaccess.h>

#define PROCFS_MAXSIZE 1024
#define PROCFS_NAME "bufferlk"
struct proc_dir_entry *tlc_proc_a;

static ssize_t proc_read(struct file *fp, char *buf, size_t len, loff_t * off)
{	
	static int finished=0; 
	if(finished) 
		{
			finished=0;
			return 0;
		} 
		finished=1;
	strcpy(buf,"Hello This is my first proc File System.");
	return strlen(buf);
}

static struct file_operations proc_fops = { .owner=THIS_MODULE, .read=proc_read, };


static int proc_init(void)
{
	printk(KERN_ALERT "Module 'proc_LKM' Loaded\n");
 	tlc_proc_a = proc_create( PROCFS_NAME , 0444, NULL, &proc_fops); 
 	if(tlc_proc_a==NULL) 
 		{	
 			printk(KERN_ALERT "Error: Could not initialize %s\n", PROCFS_NAME);
 			return -ENOMEM; 
 		}

	printk(KERN_INFO "/proc/%s Created\n", PROCFS_NAME);
		//Everything is Ok.
	return 0;
}

static void proc_exit(void) 
{  
	remove_proc_entry(PROCFS_NAME, NULL);
	printk(KERN_ALERT "Module 1 Unloaded\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saurabh Gupta");
MODULE_DESCRIPTION("/proc File System Module ");
