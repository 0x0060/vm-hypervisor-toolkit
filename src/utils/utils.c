#include "utils/utils.h"
#include "logging/logger.h"
#include <libvmi/libvmi.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void pause_vm(vmi_instance_t vmi) {
    if (vmi_pause_vm(vmi) == VMI_FAILURE) {
        log_violation("Failed to pause VM");
    }
}

void resume_vm(vmi_instance_t vmi) {
    if (vmi_resume_vm(vmi) == VMI_FAILURE) {
        log_violation("Failed to resume VM");
    }
}

bool check_vmi_init(vmi_instance_t vmi) {
    if (vmi == NULL) {
        log_violation("VMI instance is not initialized");
        return false;
    }
    return true;
}

char* get_process_name(vmi_instance_t vmi, addr_t dtb) {
    char* proc_name = NULL;
    if (vmi_get_process_name(vmi, dtb, &proc_name) == VMI_FAILURE) {
        log_violation("Failed to get process name");
        return NULL;
    }
    return proc_name;
}

vmi_pid_t get_process_pid(vmi_instance_t vmi, addr_t dtb) {
    vmi_pid_t pid = -1;
    if (vmi_dtb_to_pid(vmi, dtb, &pid) == VMI_FAILURE) {
        log_violation("Failed to get process PID");
        return -1;
    }
    return pid;
}

bool is_kernel_address(vmi_instance_t vmi, addr_t addr) {
    addr_t kernel_start = 0;
    if (vmi_get_kernel_start_address(vmi, &kernel_start) == VMI_FAILURE) {
        log_violation("Failed to get kernel start address");
        return false;
    }
    return addr >= kernel_start;
}

status_t safe_memory_access(vmi_instance_t vmi, addr_t addr, size_t size, void* buf, size_t* bytes_read) {
    if (!check_vmi_init(vmi) || !buf || !bytes_read) {
        return VMI_FAILURE;
    }

    *bytes_read = 0;
    status_t ret = vmi_read(vmi, &addr, size, buf, bytes_read);
    if (ret != VMI_SUCCESS || *bytes_read != size) {
        log_violation("Memory access failed or incomplete");
        return VMI_FAILURE;
    }

    return VMI_SUCCESS;
}