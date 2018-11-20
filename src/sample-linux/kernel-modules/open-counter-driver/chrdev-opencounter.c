/*---------------------------------------------------------------------------------------------------------------------
   open sayısını sayan bir counter
---------------------------------------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oğuz Karan");
MODULE_DESCRIPTION("Generic kernel module");

int opencounter_open(struct inode *in, struct file *filp);
int opencounter_close(struct inode *in, struct file *filp);

dev_t g_dev;
struct cdev *g_mycdev;
struct file_operations g_fops = {
    .owner = THIS_MODULE,
    .open = opencounter_open,
    .release = opencounter_close    
};


int g_count;
char g_msg[256];

static int __init generic_init(void)
{
    int result;
    
    if ((result = alloc_chrdev_region(&g_dev, 0, 1, "chrdev-opencounter")) != 0) {
        printk(KERN_ALERT "Cannot allocate device\n");
        return result;
    }
    
    printk(KERN_INFO "Success, Major:%d Minor:%d\n", MAJOR(g_dev), MINOR(g_dev)); 
    
    if ((g_mycdev = cdev_alloc()) == NULL)
        printk(KERN_ALERT "Cannot add character device\n");
    
    g_mycdev->ops = &g_fops;
    
    if (cdev_add(g_mycdev, g_dev, 1) < 0)
        printk(KERN_ALERT "Cannot add character device\n");    
    
    return 0;
}

int opencounter_open(struct inode *in, struct file *filp)
{ 
    if (g_count == 1)
        return -EINVAL;
    
    sprintf(g_msg, "Count=%d", g_count++);    
    printk(KERN_INFO "Open:%s\n", g_msg);   
    
    
    return 0;    
}

int opencounter_close(struct inode *in, struct file *filp)
{
    sprintf(g_msg, "Count=%d", g_count--);    
    printk(KERN_INFO "Close:%s\n", g_msg);
    
    return 0;    
}

static void __exit generic_exit(void)
{
    cdev_del(g_mycdev);
    unregister_chrdev_region(g_dev, 1);    
}

module_init(generic_init);
module_exit(generic_exit);
