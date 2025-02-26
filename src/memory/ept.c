#include "memory/ept.h"
#include "logging/logger.h"
#include "utils/utils.h"
#include <libvmi/libvmi.h>
#include <inttypes.h>

void setup_ept_protection(vmi_instance_t vmi) {
    addr_t kernel_text_start, kernel_text_end, kernel_rodata_start;

    if (vmi_translate_ksym2v(vmi, "_text", &kernel_text_start) == VMI_FAILURE ||
        vmi_translate_ksym2v(vmi, "_etext", &kernel_text_end) == VMI_FAILURE ||
        vmi_translate_ksym2v(vmi, "__start_rodata", &kernel_rodata_start) == VMI_FAILURE) {
        log_violation("Failed to translate kernel symbols");
        return;
    }

    for (addr_t addr = kernel_text_start; addr < kernel_text_end; addr += 0x1000) {
        if (vmi_set_page_protection(vmi, addr, VMI_PAGE_READONLY) == VMI_FAILURE) {
            log_violation("Failed to set EPT protection for kernel text");
        }
    }

    for (addr_t addr = kernel_rodata_start; addr < kernel_text_end; addr += 0x1000) {
        if (vmi_set_page_protection(vmi, addr, VMI_PAGE_READONLY) == VMI_FAILURE) {
            log_violation("Failed to set EPT protection for kernel rodata");
        }
    }

    log_violation("EPT protection enabled for kernel text and rodata");
}