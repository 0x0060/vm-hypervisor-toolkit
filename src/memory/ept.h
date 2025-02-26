#ifndef EPT_H
#define EPT_H

#include <stdint.h>
#include <stdbool.h>

#define EPT_READ        (1ULL << 0)
#define EPT_WRITE       (1ULL << 1)
#define EPT_EXECUTE     (1ULL << 2)
#define EPT_IGNORE_PAT  (1ULL << 6)
#define EPT_LARGE_PAGE  (1ULL << 7)

typedef enum {
    EPT_VIOLATION_READ = 0,
    EPT_VIOLATION_WRITE = 1,
    EPT_VIOLATION_EXECUTE = 2
} ept_violation_type;

typedef struct {
    uint64_t present : 1;
    uint64_t writable : 1;
    uint64_t executable : 1;
    uint64_t user_access : 1;
    uint64_t accessed : 1;
    uint64_t dirty : 1;
    uint64_t ignore_pat : 1;
    uint64_t large_page : 1;
    uint64_t global : 1;
    uint64_t ignored_1 : 3;
    uint64_t pfn : 40;
    uint64_t ignored_2 : 12;
} ept_entry;

typedef struct {
    void* root_table;
    uint32_t flags;
    bool enabled;
} ept_context;

int ept_init(void);
void ept_cleanup(void);

ept_context* ept_context_create(void);
void ept_context_destroy(ept_context* context);

int ept_map_page(ept_context* context, uint64_t gpa, uint64_t hpa, uint64_t flags);
int ept_unmap_page(ept_context* context, uint64_t gpa);
int ept_modify_page(ept_context* context, uint64_t gpa, uint64_t flags);

int ept_handle_violation(ept_context* context, uint64_t gpa, ept_violation_type violation);

bool ept_is_present(ept_context* context, uint64_t gpa);
uint64_t ept_get_flags(ept_context* context, uint64_t gpa);
uint64_t ept_gpa_to_hpa(ept_context* context, uint64_t gpa);

int ept_set_execute_only(ept_context* context, uint64_t gpa);
int ept_set_read_only(ept_context* context, uint64_t gpa);
int ept_invalidate_tlb(ept_context* context);

#endif // EPT_H