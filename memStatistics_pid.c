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

asmlinkage long page_present(struct mm_struct *mm,unsigned long va)
{
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep , pte;
// Using Page Tables (this mechanism is known as "Page Walk"), we find the page that corresponds to Virtual Address
    pgd = pgd_offset(mm, va);
    if (!pgd_none(*pgd) || !pgd_bad(*pgd))
    {
        pud = pud_offset((p4d_t *)pgd , va);
        if (!pud_none(*pud) || !pud_bad(*pud))
        {
            pmd = pmd_offset(pud, va);
            if (!pmd_none(*pmd) || !pmd_bad(*pmd))
            {
                ptep = pte_offset_map(pmd, va);
                if (pte_present(*ptep))
                {
			return 1;
                } else return 0; //printk(KERN_ERR, "Page Walk exception at pte entry. The Virtual Address 0x%lx cannot be translated for this process", va );
            } else return 0; //printk(KERN_ERR, "Page Walk exception at pmd entry. The Virtual Address 0x%lx cannot be translated for this process", va );
        } else return 0;//printk(KERN_ERR, "Page Walk exception at pud entry. The Virtual Address 0x%lx cannot be translated for this process", va );
    } else return 0; //printk(KERN_ERR, "Page Walk exception at pgd entry. The Virtual Address 0x%lx cannot be translated for this process", va );
return 0;
}

asmlinkage long print_vss(struct vm_area_struct *vm)
{
	unsigned long vss_vma;
		vss_vma = (vm->vm_end - vm->vm_start);
	
	vss_vma /= 1024;
	printk(KERN_INFO "VSS : %ldKB  ",vss_vma);
	return vss_vma;
}

asmlinkage long print_rss(struct task_struct *task,struct vm_area_struct *vm)
{
	unsigned long rss_vma = 0;
	unsigned long virtadd;
	for(virtadd=vm->vm_start;virtadd != vm->vm_end;virtadd++){
	if(page_present(task->mm,virtadd))
	{
		rss_vma++;
	}
	}
	rss_vma /= 1024;
	printk(KERN_INFO "RSS : %ldKB\n",rss_vma);
	return rss_vma;
}

asmlinkage void find_vss_rss(struct task_struct *task)
{
	unsigned long vss = 0;
	unsigned long rss = 0;
	if(NULL == task) return -1;		// if task is null simply return -1
	struct mm_struct *mm1 = task->mm;		
	if(NULL == mm1) return -1;
	struct vm_area_struct *vm = mm1->mmap;
	unsigned long start, end;			// 
	
	do 
	{
		printk(KERN_INFO "[%lx-%lx] VMA : ",vm->vm_start,vm->vm_end);
		vss += print_vss(vm);
		rss += print_rss(task,vm);
		vm = vm->vm_next;
	}while(NULL != vm);          // run till vm is not null
	printk(KERN_INFO "Total VSS : %ldKB Total RSS : %ldKB\n",vss,rss);
}

asmlinkage long find_wss(struct task_struct *task)
{
	long wss = 0;
	return wss;
}


asmlinkage long hook_func(int *id,char **argv)
{
	printk(KERN_INFO "hook_func function is called!!!");

	int ID = *id;
	if(ID == 0) 
	{
		printk(KERN_INFO "Bad Pid !!");
		return -1;
	}
	else
	{
		task = pid_task(find_get_pid(ID), PIDTYPE_PID);
		struct mm_struct *mm = task->mm;	

		printk("Heap\t\t\t= [%lx]-[%lx]\n",mm->start_brk,mm->brk);
		printk("Data Segment\t\t= [%lx]-[%lx]\n",mm->start_data,mm->end_data);
		printk("Code Segment\t\t= [%lx]-[%lx]\n",mm->start_code,mm->end_code);

		find_vss_rss(task);
		printk("\n--------\n---------\n");
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
	printk(KERN_ALERT "Module 1 unloaded\n");
	address = NULL;
}

module_init(LKM_1_init);
module_exit(LKM_1_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saurabh Gupta");
MODULE_DESCRIPTION("Process Memmory");