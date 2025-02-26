#include <stdio.h>
#include <stdlib.h>
#include <libvmi/libvmi.h>
#include "memory/kaslr.h"
#include "logging/logger.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <vm_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *vm_name = argv[1];
    vmi_instance_t vmi;

    if (vmi_init(&vmi, VMI_XEN, vm_name, VMI_INIT_DOMAINNAME, NULL, NULL) == VMI_FAILURE) {
        printf("Failed to initialize LibVMI\n");
        return EXIT_FAILURE;
    }

    spoof_kaslr(vmi);

    vmi_destroy(vmi);
    return EXIT_SUCCESS;
}