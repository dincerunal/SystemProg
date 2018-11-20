/*----------------------------------------------------------------------------------------------------------------------
    MODULE_XXX makroları. Aşağıdaki 3(üç) bulunması tavsiye edilir
----------------------------------------------------------------------------------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oğuz Karan");
MODULE_DESCRIPTION("Hello Module");


static __init int hello_init(void)
{
    printk(KERN_INFO "Merhaba Kernel...\n");
    printk(KERN_INFO "msg=%s, Val=%d\n", msg, val);
    
    return 0;
}

static __exit void hello_exit(void)
{
    printk(KERN_INFO "Güle güle Kernel...\n");
}

module_init(hello_init);
module_exit(hello_exit);