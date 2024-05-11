#include <sys/proc.h>

static int lab3_modcmd(modcmd_t cmd, void* arg) {
    struct proc *p;
    switch (cmd) {
        case MODULE_CMD_INIT:
            printf("Initializing lab3 driver\n");
            PROCLIST_FOREACH(p, &allproc) {
                printf("Process ID: %d, Name: %s\n", p->p_pid, p->p_comm);
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
