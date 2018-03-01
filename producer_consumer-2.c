#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/sched.h>
#include <linux/sysfs.h> 
#include <linux/fs.h> 
#include <linux/string.h> 
#include <linux/printk.h>
#include <linux/kobject.h> 



#define PROCFS_MAXSIZE 1024
#define PROCFS_NAME_PRODUCER "Producer"
#define PROCFS_NAME_CONSUMER "Consumer"

//Sys file related
#undef VERIFY_OCTAL_PERMISSIONS
#define  VERIFY_OCTAL_PERMISSIONS(perms)(perms)


const char *empty  = "Queue Empty \n";
int  flag = 1;
int finish_reading = 0;
int len,temp,i=0,ret;

unsigned long emp_len = 13;
struct Queue {
	struct list_head next;
	char *data;
};

static struct Queue *node;
struct list_head *head;
struct list_head test_head;
int new_node=1;


char *msg;

struct proc_dir_entry *producer, *consumer;
char *dest;
char proc_buf[PROCFS_MAXSIZE];

unsigned long abc = 100;

//granaluraity
int ptr = 0;
int gran = 0;
int gran1 = 1;

//statistics
int num_writes = 0;
int num_reads = 0;
int num_bytes_read = 0;
int num_bytes_write = 0;
int curr_size_queue_bytes = 0;
int num_items_queue = 0;


//sysfile related
static struct kobject *example_kobject;
static int foo = 100;

//proc file related
static ssize_t proc_read(struct file *fp, char *buf, size_t len, loff_t * off)
{
        num_reads++;
        if(list_empty(head))					//if list is empty initially 
        {
                if(flag == 1)
                {
                        ret = emp_len;
                        flag = 0;
                        printk(KERN_INFO " Entered point-0");
                }
                else if(flag == 0)
                {
                        ret = 0;
                        flag = 1;
                }
                copy_to_user(buf, empty, len);
                printk(KERN_INFO "\n Stack Empty \n");
                return ret;
        }
	
	gran1 = gran;
    node = list_first_entry(head, struct Queue, next);						//Take first element and start reading
    msg = node->data;
	dest = kmalloc((gran+1)*sizeof(char),GFP_KERNEL);
	dest[0] = '\0';
	if(finish_reading != 1)
	{														// 
	        while(gran1 != 0 )
	        {
				printk(KERN_INFO " Entered point-1");
	        	

	        	if( msg[ptr] == '\0')
	        	{
				printk(KERN_INFO " Entered point-2");
				list_del(&(node->next));
	        		if(list_empty(head))
	        		{
						printk(KERN_INFO " Entered point-3");

						printk(KERN_INFO "\n No Elements left. \n");
						printk(KERN_INFO "");
						ptr = 0;
	        			return 0;
	        		}

	        		node = list_first_entry(head, struct Queue, next);
	        		msg = node->data;
					ptr = 0;
	        	}



				printk(KERN_INFO " Entered point-4");
				gran1--;
				strncat(dest,(msg + ptr),1);
				printk(KERN_INFO "dest till now : %s its len = %d",dest,strlen(dest));
				printk(KERN_INFO " ");

				ptr++;

	        }
       	dest[gran] = '\0';
		copy_to_user(buf,dest,gran+1);
		printk(KERN_INFO "Buf value : %s",buf);
		printk(KERN_INFO " Entered point-5");
		printk(KERN_INFO " ");
		finish_reading = 1;
		return gran;
}
if(finish_reading == 1){
		printk(KERN_INFO " Entered point-6");
		//gran1 = gran;
		finish_reading = 0;
	return 0;
	}
// 			if(finish_reading  != 1)
	// {
	// 	printk(KERN_INFO "\nentered point-1\n");
	// 	if(gran == 0)
	// 	{
	// 		printk(KERN_INFO "\nentered point-2\n");
	// 	   	node = list_first_entry(head, struct Queue, next);
 //            		msg = node->data;
 //            		ret = strlen(msg);
 //            		num_bytes_read += ret;
	// 		sprintf(buf,msg);
 //            		/*if(copy_to_user(buf, msg, len))
 //            		{
 //                    		return -EFAULT;
 //            		}*/
 //        		printk(KERN_INFO "\n Data : %s\n",msg);
 //            		list_del(&(node->next));
 //            		num_items_queue--;
 //            		curr_size_queue_bytes -= ret;
	//         	finish_reading = 1;
	// 		ptr = 0;
	//         	return ret;
	// 	}
	// 	else
	// 	{
	// // 	 printk(KERN_INFO "\nentered point-3\n");
	// 	// 	node = list_first_entry(head, struct Queue, next);
 //  		//  msg = node->data;
	// 	// 	char *dest;
	// 	// 	dest = kmalloc((gran+1)*sizeof(char),GFP_KERNEL);
	// 	// 	int gran1 = gran;
	// 	// 	int gran2;
	// 	// 	while(gran1 != 0){
	// 	// 		if((ptr + gran1) > strlen(msg))
	// 	// 		{

	// 	// 			printk(KERN_INFO "\nentered point-4\n");
	// 	// 		    list_del(&(node->next));

	// 	// 			num_items_queue--;
	// 	// 			strcat(dest,msg+ptr);
	// 	// 			gran1 -= strlen(msg+ptr);
	// 	// 			printk(KERN_INFO "number of characters left to be printed is : %d",gran1);
	// 	// 			printk(KERN_INFO " ");
	// 	// 			ptr = 0;

	// 	// 			if(list_empty(head))
	//  //        		{
	// 	// 				copy_to_user(buf, empty, len);
	//  //                			printk(KERN_INFO "\n Stack Empty \n");
	// 	// 				return 13;
	// 	// 			}
	// 	// 			node = list_first_entry(head, struct Queue, next);
	//  //                msg = node->data;
	//  //                if(gran1 > strlen(msg))
	//  //                {	
	//  //                	printk(KERN_INFO "\nentered point-7\n");
	//  //                	gran2 = strlen(msg);
	// 	// 			}
	// 	// 			else{
	// 	// 				printk(KERN_INFO "\nentered point-8\n");
	// 	// 				gran2 = gran1;
	// 	// 			}	
	// 	// 			strncat(dest, msg+ptr, gran2);
	// 	// 			ptr += gran2;
	// 	// 			gran1 -= gran2;

	// 	// 		}else{

	// 	// 			printk(KERN_INFO "\nentered point-5\n");

	// 	// 			strncat(dest, msg+ptr, gran1);
	// 	// 			ptr += gran1;
	// 	// 			gran1 = 0 ;
	// 	// 		}

	// 	// 	}
	// 	// 		dest[gran] = '\0';
	// 	// 		sprintf(buf,dest);
	// 	// 		finish_reading = 1;
	// 	// 		num_bytes_read += (strlen(dest)+1);
	// 	// 		curr_size_queue_bytes -= (strlen(dest)+1);
	// 	// 		num_bytes_read += strlen(dest);
	// 	// 		printk(KERN_INFO "\n Data : %s\n",dest);
	// 	// 		return strlen(dest);
	// 	// }
	// }
	// else if(finish_reading == 1)
	// {

	// 	printk(KERN_INFO "\nentered point-6\n");
	// 	finish_reading = 0;
	// 	return 0;
	// }
}
/*static ssize_t proc_read(struct file *fp, char *buf, size_t len, loff_t * off)
{	
	num_reads++;
	if(list_empty(head))
	{
		if(flag == 1)
		{
			ret = emp_len;
			flag = 0;
		}
		else if(flag == 0)
		{
			ret = 0;
			flag = 1;
		}
		copy_to_user(buf, empty, len);
		printk(KERN_INFO "\n Stack Empty \n");
		return ret;
	}
	if(new_node == 1)
	{
		node = list_first_entry(head, struct Queue, next);
		msg = node->data;
		ret = strlen(msg);
		num_bytes_read += ret;
		new_node = 0;
	}
	if(len > ret)
	{
		len = ret;
	}
	if(ret != 0){
		if(copy_to_user(buf, msg, len))
        	{
			return -EFAULT;
        	}
	printk(KERN_INFO "\n Data : %s\n",msg);
	}
	ret = ret - len;
	if(len == 0)
	{
		list_del(&(node->next));
		num_items_queue--;
		curr_size_queue_bytes -= strlen(msg);
		new_node = 1;
	}
	return len;
}*/

static ssize_t proc_write(struct file *fp, const char *buf, size_t len, loff_t * off)
{
	
	num_writes++;
	if(len > PROCFS_MAXSIZE) 
	{
		printk(KERN_ALERT "Trying to write more data as specified.!!!\n");
		return -EFAULT;
	}

	msg = kmalloc((len)*sizeof(char),GFP_KERNEL);

	if(copy_from_user(msg, buf, len))
	{
		return -EFAULT;
	}
	msg[len-1] = '\0' ;
	printk(KERN_INFO "size of buffer is %d\n",len);
	node = kmalloc(sizeof(struct Queue *),GFP_KERNEL);
	node->data = msg;
	printk(KERN_INFO "Value written by producer module is %s\n ",node->data);
	num_bytes_write += strlen(msg) + 1;
	list_add_tail(&(node->next),head);
	num_items_queue++;
	curr_size_queue_bytes += strlen(msg) + 1;
	
	printk(KERN_INFO " ");
	return len;
}
static struct file_operations producer_fops = { .owner=THIS_MODULE, .write=proc_write};

static struct file_operations consumer_fops = { .owner=THIS_MODULE, .read=proc_read};





//sysfile related
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
        return sprintf(buf, "\nNumber of Reads : %d\n Number of Writes : %d \n,Number of items in Queue %d\n Number of Bytes Read: %dB\n Number of Bytes Written : %dB\n Cuurent Size of Queue in Bytes %dB\n", num_reads, num_writes, num_items_queue, num_bytes_read, num_bytes_write, curr_size_queue_bytes);
	//return sprintf(buf,"%d\n",foo);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                      const char *buf, size_t count)
{
        sscanf(buf, "%d", &gran);
	printk(KERN_INFO "Entered Granalurity is : %d",gran);
	printk(KERN_INFO " ");
        return count;
}


static struct kobj_attribute foo_attribute = __ATTR(foo, 0666, foo_show, foo_store);

static int proc_init(void)
{
	int error = 0;

	printk(KERN_ALERT "Module 'Producer_Consumer' Loaded\n");

	head = kmalloc(sizeof(struct list_head *),GFP_KERNEL);
	INIT_LIST_HEAD(head);
	emp_len = strlen(empty);
 	producer = proc_create( PROCFS_NAME_PRODUCER , 0666, NULL, &producer_fops); 
 	consumer = proc_create( PROCFS_NAME_CONSUMER , 0666, NULL, &consumer_fops); 
 	if(producer == NULL || consumer == NULL) 
 		{	
 			printk(KERN_ALERT "Error: Could not initialize %s\n", PROCFS_NAME_PRODUCER);
 			printk(KERN_ALERT "Error: Could not initialize %s\n", PROCFS_NAME_CONSUMER);
 			return -ENOMEM; 
 		}

	printk(KERN_INFO "/proc/%s Created\n", PROCFS_NAME_PRODUCER);
	printk(KERN_INFO "/proc/%s Created\n", PROCFS_NAME_CONSUMER);
		//Everything is Ok.




	 pr_debug("Module initialized successfully \n");

        example_kobject = kobject_create_and_add("kobject_example",
                                                 kernel_kobj);
        if(!example_kobject)
                return -ENOMEM;

        error = sysfs_create_file(example_kobject, &foo_attribute.attr);
        if (error) {
                pr_debug("failed to create the foo file in /sys/kernel/kobject_example \n");
        }

        return error;

}

static void proc_exit(void) 
{  
	remove_proc_entry(PROCFS_NAME_PRODUCER, NULL);
	remove_proc_entry(PROCFS_NAME_CONSUMER, NULL);
	pr_debug ("Module uninitialized successfully \n");
        kobject_put(example_kobject);
	printk(KERN_ALERT "Model 'Producer_Consumer' Unloaded\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saurabh Gupta");
MODULE_DESCRIPTION("/proc File System Module ");
