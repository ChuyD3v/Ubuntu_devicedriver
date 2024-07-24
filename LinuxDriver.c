#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>  // Para copy_to_user y copy_from_user

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jesus Velez");
MODULE_DESCRIPTION("Descripción del módulo");

static struct proc_dir_entry *custom_proc_node;

// Declaración del prototipo de la función
ssize_t driver_read(struct file *file_ptr, char __user *user_space_buff, size_t count, loff_t *offset);
//ssize_t driver_write(struct file *file_ptr, char __user *user_space_buff, size_t count, loff_t *offset);

struct proc_ops driver_proc_ops = {
    //.proc_write = 
    .proc_read = driver_read,
};

ssize_t driver_read(struct file *file_ptr, char __user *user_space_buff, size_t count, loff_t *offset) {
    printk(KERN_INFO "driver read\n");

    char msg[] = "Ack\n";
    size_t len = strlen(msg);
    int result;

    if(*offset >= len){
        return 0;
    }

    result =copy_to_user(user_space_buff,msg,len);
    *offset += len;

    return len;
}

static int __init init_mod(void) {
    printk(KERN_INFO "init: entry\n");

    custom_proc_node = proc_create("dev_driver", 0, NULL, &driver_proc_ops);

    if (custom_proc_node == NULL) {
        printk(KERN_ERR "module: error creating proc entry\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "init: exit\n");
    return 0;
}

static void __exit exit_mod(void) {
    printk(KERN_INFO "exit: entry\n");

    if (custom_proc_node) {
        proc_remove(custom_proc_node);
    }

    printk(KERN_INFO "exit: exit\n");
}

module_init(init_mod);
module_exit(exit_mod);
