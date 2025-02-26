#ifndef KASLR_H
#define KASLR_H

#include <libvmi/libvmi.h>
#include <inttypes.h>

void handle_kaslr_spoofing(vmi_instance_t vmi);

addr_t calculate_kaslr_offset(vmi_instance_t vmi);

void spoof_kaslr_offset(vmi_instance_t vmi, addr_t kaslr_offset);

#endif // KASLR_H