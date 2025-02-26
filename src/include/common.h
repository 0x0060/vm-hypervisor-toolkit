#ifndef _COMMON_H_
#define _COMMON_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
#error "Linux kernel version 4.0.0 or higher is required"
#endif

#define STATUS_SUCCESS          0
#define STATUS_FAILED          -1
#define STATUS_INVALID_PARAM   -2
#define STATUS_NO_MEMORY      -3
#define STATUS_NOT_SUPPORTED  -4

#define LOG_LEVEL_ERROR    0
#define LOG_LEVEL_WARN     1
#define LOG_LEVEL_INFO     2
#define LOG_LEVEL_DEBUG    3
#define LOG_LEVEL_TRACE    4

#define MAX_CPU_CORES      256
#define MAX_VM_COUNT       64
#define PAGE_SIZE          4096
#define MAX_PATH_LENGTH    256

#define MEM_READ           (1 << 0)
#define MEM_WRITE          (1 << 1)
#define MEM_EXECUTE        (1 << 2)
#define MEM_USER           (1 << 3)
#define MEM_KERNEL         (1 << 4)

#define INT_EXTERNAL       (1 << 0)
#define INT_NMI            (1 << 1)
#define INT_EXCEPTION      (1 << 2)
#define INT_SOFT           (1 << 3)

#define VM_EXIT_EXCEPTION  0x0
#define VM_EXIT_EXTERNAL   0x1
#define VM_EXIT_TRIPLE     0x2
#define VM_EXIT_INIT       0x3
#define VM_EXIT_SIPI       0x4
#define VM_EXIT_IO_SMI     0x5
#define VM_EXIT_SMI        0x6
#define VM_EXIT_IRQ        0x7

#define MEM_TYPE_UC        0x0
#define MEM_TYPE_WC        0x1
#define MEM_TYPE_WT        0x4
#define MEM_TYPE_WP        0x5
#define MEM_TYPE_WB        0x6
#define MEM_TYPE_UC_MINUS  0x7

typedef struct {
    uint64_t guest_phys;
    uint64_t host_phys;
    uint64_t size;
    uint32_t flags;
} memory_region_t;

typedef struct {
    uint32_t vcpu_id;
    uint32_t cpu_id;
    uint64_t cr0;
    uint64_t cr3;
    uint64_t cr4;
    uint64_t rflags;
} vcpu_state_t;

typedef struct {
    uint32_t vm_id;
    uint32_t vcpu_count;
    memory_region_t *regions;
    uint32_t region_count;
    void *private_data;
} vm_context_t;

typedef int status_t;

#define ALIGN_UP(x, align)     (((x) + ((align) - 1)) & ~((align) - 1))
#define ALIGN_DOWN(x, align)   ((x) & ~((align) - 1))
#define IS_ALIGNED(x, align)   (((x) & ((align) - 1)) == 0)
#define MIN(a, b)             ((a) < (b) ? (a) : (b))
#define MAX(a, b)             ((a) > (b) ? (a) : (b))

#define BIT(n)              (1ULL << (n))
#define BITS_PER_LONG       64
#define GENMASK(h, l)       (((~0ULL) << (l)) & (~0ULL >> (BITS_PER_LONG - 1 - (h))))

#endif