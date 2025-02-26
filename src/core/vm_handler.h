#ifndef VM_HANDLER_H
#define VM_HANDLER_H

#include <linux/types.h>
#include "common.h"

#define VM_STATE_STOPPED    0
#define VM_STATE_RUNNING    1
#define VM_STATE_PAUSED     2
#define VM_STATE_ERROR      3

typedef struct {
    uint32_t memory_size;
    uint32_t vcpu_count;
    uint32_t flags;
} vm_config_t;

typedef struct {
    uint32_t vm_id;
    uint32_t state;
    vm_config_t config;
    void *private_data;
} vm_instance_t;

status_t vm_init(void);
status_t vm_cleanup(void);

status_t vm_create(vm_config_t *config, uint32_t *vm_id);
status_t vm_destroy(uint32_t vm_id);

status_t vm_start(uint32_t vm_id);
status_t vm_stop(uint32_t vm_id);
status_t vm_pause(uint32_t vm_id);
status_t vm_resume(uint32_t vm_id);

status_t vm_get_state(uint32_t vm_id, uint32_t *state);

status_t vm_set_memory_region(uint32_t vm_id, memory_region_t *region);
status_t vm_protect_memory_region(uint32_t vm_id, uint64_t guest_phys, uint64_t size);

status_t vm_enable_monitoring(uint32_t vm_id, uint32_t flags);
status_t vm_disable_monitoring(uint32_t vm_id, uint32_t flags);

status_t vm_dump_state(uint32_t vm_id);
status_t vm_get_statistics(uint32_t vm_id, void *stats, uint32_t size);

#endif