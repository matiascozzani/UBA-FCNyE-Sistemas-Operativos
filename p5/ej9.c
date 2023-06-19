#define MODULE
#define __KERNEL__
struct file_operations memoria_fops = {
    read : memoria_read,
    write : memoria_write,
    open : memoria_open,
    release : memoria_release
}; // definimos la interfaz del driver.
int memoria_major = 60;
char *memoria_buffer;
int init_module(void)
{
    int result;
    result = register_chrdev(memoria_major, "memoria", &memoria_fops);
    if (result < 0)
    {
        printk("<1>memoria: no puedo obtener numero mayor %d\n", memoria_major);
        return result;
    }
    memoria_buffer = kmalloc(1, GFP_KERNEL);
    if (!memoria_buffer)
    {
        result = -ENOMEM;
        goto fallo;
    }
    memset(memoria_buffer, 0, 1); // inicializo un buffer y lo lleno con un 0.
    printk("<1>Insertando modulo\n");
    return 0;
fallo:
    cleanup_module();
    return result;
}
void cleanup_module(void)
{
    unregister_chrdev(memoria_major, "memoria");
    if (memoria_buffer)
    {
        kfree(memoria_buffer);
    }
    printk("<1>Quitando modulo\n");
}
int memoria_open(struct inode *inode, struct file *filp)
{
    MOD_INC_USE_COUNT; // <- incrementa la cantidad de usuarios actuales del modulo
    return 0;
}
int memoria_release(struct inode *inode, struct file *filp)
{
    MOD_DEC_USE_COUNT; // <- decrementa la cantidad de usuarios actuales del modulo
    return 0;
}
ssize_t memoria_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{
    copy_to_user(buf, memoria_buffer, 1);// <- copia el valor de memoriabuffer a buff
    if (*offset == 0) // sumamos al offset para que si se vuelve a leer, se lea desde 1
    {
        *offset += 1;
        return 1;
    }
    else
    {
        return 0;
    }
}
ssize_t memoria_write(struct file *filp, char *buf, size_t count, loff_t *offset)
{
    char *tmp;
    tmp = buf + count - 1; //<- obtengo el último byte
    copy_from_user(memoria_buffer, tmp, 1); // <- lo guardo
    return 1;
}

/*
    El código implementa un char-device que permite compartir caracteres
    entre los usuarios del módulo. Solo se puede realizar UNA lectura.

*/