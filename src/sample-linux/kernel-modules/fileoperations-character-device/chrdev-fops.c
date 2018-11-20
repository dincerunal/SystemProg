/*----------------------------------------------------------------------------------------------------------------------
    
----------------------------------------------------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

int generic_open(struct inode *, struct file *);
int generic_release(struct inode *, struct file *);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oğuz Karan");
MODULE_DESCRIPTION("Generic Character Device");


dev_t g_dev;
struct cdev g_cdev;

struct file_operations g_fops = {
    .owner = THIS_MODULE,
    .open = generic_open,
    .release = generic_release
};

static __init int generic_device_init(void)
{
    int result;
    
    if ((result = alloc_chrdev_region(&g_dev, 0, 1, "chrdev-fops")) != 0) {
        printk(KERN_INFO "Error:result=%d\n", result);
        return result;
    }
    
    printk(KERN_INFO "Success Major:%d, Minor:%d\n", MAJOR(g_dev), MINOR(g_dev)); 
    
    cdev_init(&g_cdev, &g_fops);
    
    if ((result = cdev_add(&g_cdev, g_dev, 1)) < 0) {
        unregister_chrdev_region(g_dev, 1);
        printk(KERN_INFO "Can not add character device\n");
        return result;
    }    
    
    return 0;
}

int generic_open(struct inode *in, struct file *f)
{
    printk(KERN_INFO "open...\n");
    
    return 0;    
}
int generic_release(struct inode *in, struct file *f)
{
    printk(KERN_INFO "close...\n");
    
    return 0;
}


static __exit void generic_device_exit(void)
{
    unregister_chrdev_region(g_dev, 1);
    printk(KERN_INFO "Güle güle Kernel...\n");
}

module_init(generic_device_init);
module_exit(generic_device_exit);