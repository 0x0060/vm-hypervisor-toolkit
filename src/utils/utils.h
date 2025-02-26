#ifndef UTILS_H
#define UTILS_H

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/fs.h>

status_t utils_vm_create(vm_context_t *vm);
status_t utils_vm_destroy(vm_context_t *vm);

status_t utils_get_process_info(pid_t pid, char *name, size_t size);
status_t utils_get_thread_info(pid_t pid, thread_info_t *info);

status_t utils_verify_memory(void *addr, size_t size);
status_t utils_check_permissions(void *addr, uint32_t flags);
status_t utils_validate_buffer(const void *buf, size_t size);

status_t utils_copy_string(char *dst, const char *src, size_t size);
status_t utils_compare_buffer(const void *buf1, const void *buf2, size_t size);

status_t utils_handle_error(int error_code);
status_t utils_log_error(const char *msg);

status_t utils_alloc_resource(void **ptr, size_t size);
status_t utils_free_resource(void *ptr);

status_t utils_check_version(uint32_t version);
status_t utils_get_version_string(char *buf, size_t size);

status_t utils_get_cpu_info(cpu_info_t *info);
status_t utils_get_memory_info(memory_info_t *info);

#endif /* UTILS_H */