#include "cpu/cr0.h"
#include "logging/logger.h"
#include <libvmi/libvmi.h>
#include <inttypes.h>

#define CR0_PE 0   // Protection Enable
#define CR0_MP 1   // Monitor Coprocessor
#define CR0_EM 2   // Emulation
#define CR0_TS 3   // Task Switched
#define CR0_ET 4   // Extension Type
#define CR0_NE 5   // Numeric Error
#define CR0_WP 16  // Write Protect
#define CR0_AM 18  // Alignment Mask
#define CR0_NW 29  // Not Write-through
#define CR0_CD 30  // Cache Disable
#define CR0_PG 31  // Paging

static uint64_t get_cr0_value(vmi_instance_t vmi) {
    uint64_t cr0_value = 0;
    if (vmi_get_vcpureg(vmi, &cr0_value, CR0, 0) == VMI_FAILURE) {
        log_violation("Failed to read CR0 value");
        return 0;
    }
    return cr0_value;
}

void protect_cr0(vmi_instance_t vmi) {
    uint64_t initial_cr0 = get_cr0_value(vmi);
    if (!initial_cr0) {
        return;
    }

    if (!(initial_cr0 & (1ULL << CR0_PE))) {
        log_violation("Protection mode is not enabled in CR0");
    }

    if (!(initial_cr0 & (1ULL << CR0_PG))) {
        log_violation("Paging is not enabled in CR0");
    }

    if (vmi_monitor_cr0_writes(vmi) == VMI_FAILURE) {
        log_violation("Failed to monitor CR0 writes");
        return;
    }

    log_violation("CR0 write protection enabled");
}

bool validate_cr0_state(vmi_instance_t vmi) {
    uint64_t cr0 = get_cr0_value(vmi);
    if (!cr0) {
        return false;
    }

    bool valid = true;
    if (!(cr0 & (1ULL << CR0_PE))) {
        log_violation("CR0.PE (Protection Enable) is not set");
        valid = false;
    }
    if (!(cr0 & (1ULL << CR0_WP))) {
        log_violation("CR0.WP (Write Protect) is not set");
        valid = false;
    }
    if (!(cr0 & (1ULL << CR0_PG))) {
        log_violation("CR0.PG (Paging) is not set");
        valid = false;
    }
    if (cr0 & (1ULL << CR0_CD)) {
        log_violation("CR0.CD (Cache Disable) is set");
        valid = false;
    }

    return valid;
}

status_t cr0_init(void) {
    uint64_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));

    if (!(cr0 & (1ULL << CR0_PE))) {
        log_error(CR0_COMPONENT, "Protection mode not enabled");
        return STATUS_FAILED;
    }

    if (!(cr0 & (1ULL << CR0_PG))) {
        log_error(CR0_COMPONENT, "Paging not enabled");
        return STATUS_FAILED;
    }

    return STATUS_SUCCESS;
}

status_t cr0_monitor_writes(void) {
    uint64_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));

    if (!(cr0 & (1ULL << CR0_WP))) {
        log_error(CR0_COMPONENT, "Write protection not enabled");
        return STATUS_FAILED;
    }

    return STATUS_SUCCESS;
}

status_t cr0_check_security(void) {
    uint64_t cr0;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(cr0));

    if (!(cr0 & (1ULL << CR0_PE)) ||
        !(cr0 & (1ULL << CR0_WP)) ||
        !(cr0 & (1ULL << CR0_PG))) {
        log_error(CR0_COMPONENT, "Critical security bits not set");
        return STATUS_FAILED;
    }

    return STATUS_SUCCESS;
}