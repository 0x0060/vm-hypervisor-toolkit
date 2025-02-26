#include "memory/kaslr.h"
#include "logging/logger.h"
#include <linux/mm.h>

static uint64_t kernel_base_addr = 0;
static uint64_t kaslr_offset = 0;

status_t kaslr_init(void) {
    struct page *dummy_page;
    pte_t *pte;
    uint64_t phys_addr;

    kernel_base_addr = get_kernel_base();
    if (!kernel_base_addr) {
        log_error(KASLR_COMPONENT, "Failed to get kernel base address");
        return STATUS_FAILED;
    }

    kaslr_offset = calculate_kaslr_offset(kernel_base_addr);
    if (!kaslr_offset) {
        log_error(KASLR_COMPONENT, "Failed to calculate KASLR offset");
        return STATUS_FAILED;
    }

    dummy_page = alloc_page(GFP_KERNEL);
    if (!dummy_page) {
        log_error(KASLR_COMPONENT, "Failed to allocate dummy page");
        return STATUS_NO_MEMORY;
    }

    phys_addr = page_to_phys(dummy_page);
    pte = get_pte(current->mm, kernel_base_addr);
    if (!pte) {
        __free_page(dummy_page);
        log_error(KASLR_COMPONENT, "Failed to get PTE");
        return STATUS_FAILED;
    }

    set_pte(pte, pfn_pte(phys_addr >> PAGE_SHIFT, PAGE_KERNEL));
    __flush_tlb_all();

    return STATUS_SUCCESS;
}

uint64_t kaslr_get_offset(void) {
    return kaslr_offset;
}

status_t kaslr_spoof_offset(uint64_t new_offset) {
    if (!new_offset) {
        return STATUS_INVALID_PARAM;
    }

    kaslr_offset = new_offset;
    return STATUS_SUCCESS;
}