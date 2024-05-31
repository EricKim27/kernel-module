#include <linux/module.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Kim");
MODULE_DESCRIPTION("A module that intentionally causes segmentation fault inside the kernel space");

/*
 * Intentional Segmentation fault
 * This code uses NULL pointer to trigger a segmentation fault.
 * From my test environment(Fedora 40 with linux 6.8.10), this code causes oops and the kernel doesn't panic.
 */

static int __init initial(void)
{
	int *nullptr = NULL;
	int c = *nullptr;
	printk(KERN_INFO "%d\n", c);
	return 0;
}
static void __exit exitcode(void)
{
	printk(KERN_INFO "exiting\n");
}
module_init(initial);
module_exit(exitcode);

