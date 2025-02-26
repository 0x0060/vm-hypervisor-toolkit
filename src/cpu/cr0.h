#ifndef CR0_H
#define CR0_H

#include <linux/types.h>

#define CR0_PE          0x00000001
#define CR0_MP          0x00000002
#define CR0_EM          0x00000004
#define CR0_TS          0x00000008
#define CR0_ET          0x00000010
#define CR0_NE          0x00000020
#define CR0_WP          0x00010000
#define CR0_AM          0x00040000
#define CR0_NW          0x20000000
#define CR0_CD          0x40000000
#define CR0_PG          0x80000000

status_t cr0_init(void);
status_t cr0_set_bits(uint64_t bits);
status_t cr0_clear_bits(uint64_t bits);
status_t cr0_read(uint64_t *value);
status_t cr0_write(uint64_t value);
status_t cr0_verify_state(void);
status_t cr0_handle_vm_exit(vcpu_state_t *state);

#endif // CR0_H