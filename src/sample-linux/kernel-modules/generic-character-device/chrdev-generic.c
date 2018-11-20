/*----------------------------------------------------------------------------------------------------------------------
    Aşağıdaki örnekte genel bir aygıt sürücüsü için major numaranın dinamik tahsis edilmiştir. 
    Bu dinamik tahsis edilen numarayı da alıp sisyeme yükleyen ve /dev dizininde device için
    dosya açan bash script aşağıdaki gibi olabilir:
    
    #!/bin/sh

    module=$1
    mode="666"

    rm -f /dev/$module

    /sbin/insmod ./$module.ko $* || exit 1
    major=`awk "\\$2==\"$module\" {print \\$1}" /proc/devices`

    mknod /dev/$module c $major 0
    chmod $mode /dev/$module
----------------------------------------------------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oğuz Karan");
MODULE_DESCRIPTION("Generic Character Device");

dev_t g_dev;

static __init int generic_device_init(void)
{
    int result;
    
    if ((result = alloc_chrdev_region(&g_dev, 0, 1, "chrdev-generic")) != 0) {
        printk(KERN_INFO "Error:result=%d\n", result);
        return result;
    }
    
    printk(KERN_INFO "Success Major:%d, Minor:%d\n", MAJOR(g_dev), MINOR(g_dev));  
    
    
    return 0;
}

static __exit void generic_device_exit(void)
{
    unregister_chrdev_region(g_dev, 1);
    printk(KERN_INFO "Güle güle Kernel...\n");
}

module_init(generic_device_init);
module_exit(generic_device_exit);