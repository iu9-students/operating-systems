#include <sys/cdefs.h>
#include <sys/module.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <uvm/uvm.h>

MODULE(MODULE_CLASS_MISC, lab4, NULL);
#define PAGESIZE 0x1000
extern paddr_t avail_end;
vaddr_t va;
struct pglist plist;

static int lab4_modcmd(modcmd_t cmd, void* arg) {
    va = uvm_km_alloc(kernel_map, PAGESIZE*10, 0, UVM_KMF_VAONLY);
    if (va == 0) {
        return 0;
    }
    
    int error = uvm_pglistalloc(PAGESIZE*6, 0, avail_end, 0, 0, &plist, 5, 0);
    
    if (!error) printf ("LAB4 loaded\n");
    struct vm_page *page = TAILQ_FIRST(&plist);

    for (int i = 0; page; ++i) {
        pd_entry_t *ppte;
        ppte = L2_BASE+pl2_i(va+PAGESIZE*i);
        paddr_t pa = VM_PAGE_TO_PHYS(page);
        printf("Page - %d\n", i+1);
        printf("Valid - %d\n", ((*ppte & PG_V) ? 1 : 0));
        printf("Used - %d\n", ((*ppte & PG_U) ? 1 : 0));
        printf("Modified - %d\n", ((*ppte & PG_M) ? 1 : 0));
        printf("Physical address - 0x%lx\n", pa);
        printf("\n");
        page = TAILQ_NEXT(page, pageq.queue);
   }
   
   uvm_pglistfree(&plist);
   uvm_km_free(kernel_map, va, PAGESIZE*10, UVM_KMF_VAONLY);
   
   return 0;
}
