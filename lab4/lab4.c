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
    
    int error = uvm_pglistalloc(PAGESIZE*5, 0, avail_end, 0, 0, &plist, 5, 0);
    
    if (error) {
        return error;
    }

    struct vm_page *page = TAILQ_FIRST(&plist);

    for (int i = 0; page; ++i) {
        pd_entry_t *ppte;
        ppte = L2_BASE+pl2_i(va+PAGESIZE*i);
        paddr_t pa = VM_PAGE_TO_PHYS(page);
        printf("Page - %d\n", i+1);
        printf("Valid - %s\n", ((*ppte & PG_V) ? "true" : "false"));
        printf("Used - %s\n", ((*ppte & PG_U) ? "true" : "false"));
        printf("Modified - %s\n", ((*ppte & PG_M) ? "true" : "false"));
        printf("Physical address - 0x%lx\n", pa);
        printf("\n");
        page = TAILQ_NEXT(page, pageq.queue);
   }
   
    // Цикл для обработки всех выделенных виртуальных страниц
    for (int i = 0; i < 10; ++i) {
        vaddr_t current_va = va + PAGESIZE*i;
        pd_entry_t *ppte = L2_BASE + pl2_i(current_va);
        
        printf("Virtual Address: 0x%lx\n", current_va);
        printf("Valid - %s\n", ((*ppte & PG_V) ? "true" : "false"));
        printf("Used - %s\n", ((*ppte & PG_U) ? "true" : "false"));
        printf("Modified - %s\n", ((*ppte & PG_M) ? "true" : "false"));
        printf("\n");
    }
   
   uvm_pglistfree(&plist);
   uvm_km_free(kernel_map, va, PAGESIZE*10, UVM_KMF_VAONLY);
   
   return 0;
}
