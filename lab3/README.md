# Гайд на лаобраторную работу номер 3

## ReactOS

## NetBSD

1. Запускайте NetBSD на виртуалке, все дела и начинаем

2. `cd /usr/src/sys/dev` - 

3. `vi lab3.c`

4. вписываете вот эту тему внутрь файла:

```C
#include <sys/module.h>
MODULE(MODULE_CLASS_MISC,lab3,NULL);

static int lab3_modcmd(modcmd_t cmd, void* arg) {
    struct proc *p;
    switch (cmd) {
        case MODULE_CMD_INIT:
            printf("Initializing lab3 driver\n");
            // Code to get the process list and iterate through it
            PROCLIST_FOREACH(p, &allproc) {
                printf("Process ID: %d, Name: %s\n", p->p_pid, p->p_comm);
                // Additional processing if needed
            }
            break;
        case MODULE_CMD_FINI:
            printf("Shutting down lab3 driver\n");
            break;
        default:
            return ENOTTY;
    }
    return 0;
}
```

5. Esc и пишите :wq (сохранили файл и вышли из него)

6. 

```bash
cd /usr/src/sys/modules
mkdir lab3
cd lab3
touch Makefile
```

7. 

```make
.include "../Makefile.inc"
KMOD=lab3
.PATH: ${S}/dev
SRCS=lab3.c

.include <bsd.kmodule.mk>
```

8. 
```bash
cd /usr/src/sys/modules/lab3
make
```

9. 
```bash
modload ./lab3.kmod
```