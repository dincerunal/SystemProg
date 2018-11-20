/*---------------------------------------------------------------------------------------------------------------------
   Bir diziden rasgele sayı üreten bir driver
---------------------------------------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oğuz Karan");
MODULE_DESCRIPTION("Random generator driver");

int randomgenerator_open(struct inode *in, struct file *filp);
int randomgenerator_close(struct inode *in, struct file *filp);
ssize_t randomgenerator_read (struct file *filp, char *buf, size_t count, loff_t *offset);
ssize_t randomgenerator_write (struct file *filp, const char *buf, size_t count, loff_t *offset);
loff_t randomgenerator_lseek(struct file *filp, loff_t offset, int whence);

dev_t g_dev;
struct cdev *g_mycdev;
struct file_operations g_fops = {
    .owner = THIS_MODULE,
    .open = randomgenerator_open,
    .read = randomgenerator_read,
    .write = randomgenerator_write,
    .llseek = randomgenerator_lseek,
    .release = randomgenerator_close    
};

int g_numbers[30];
int g_index;

static int __init generic_init(void)
{
    int result;
    
    if ((result = alloc_chrdev_region(&g_dev, 0, 1, "chrdev-random2")) != 0) {
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

int randomgenerator_open(struct inode *in, struct file *filp)
{    
    int i;
     
    for (i = 0; i < 30; ++i)
        get_random_bytes(&g_numbers[i], sizeof(int));
    
    return 0;    
}

ssize_t randomgenerator_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{
    int val;
    char msg[256];
    int number_of_read_bytes = 0;
    
    if (g_index == 30)
        return 0;    
    
    val = g_numbers[g_index++];
    val %= 100;
    
    printk(KERN_INFO "val=%d\n", val);
    sprintf(msg, "%d", val);    
    
    while (number_of_read_bytes < count && msg[number_of_read_bytes] != '\0')
        put_user(msg[number_of_read_bytes++], buf++); /* user seviyesindeki bellek alanına yazmak için kullanılabilir */    
    
    return number_of_read_bytes;    
}

ssize_t randomgenerator_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
    printk(KERN_INFO "write function...\n");            
    
    return 0;
}

int randomgenerator_close(struct inode *in, struct file *filp)
{   
    g_index = 0;    
    return 0;    
}

loff_t randomgenerator_lseek(struct file *filp, loff_t offset, int whence)
{
    return 0;
}

static void __exit generic_exit(void)
{    
    cdev_del(g_mycdev);
    unregister_chrdev_region(g_dev, 1);    
}

module_init(generic_init);
module_exit(generic_exit);
