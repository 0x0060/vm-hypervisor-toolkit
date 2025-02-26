#ifndef _RESOLVE_KALLSYMS_H_
#define _RESOLVE_KALLSYMS_H_

#include <linux/kprobes.h>
#include <linux/ftrace.h>
#include <asm/unistd.h>

typedef uintptr_t (*resolve_symbol_t)(const char *symbol_name);
static resolve_symbol_t resolve_symbol_func = NULL;

uintptr_t get_symbol_address(const char *func_name) {
    static struct kprobe probe;
    probe.symbol_name = func_name;
    if (register_kprobe(&probe) < 0) {
        pr_info("Failed to resolve symbol: %s\n", func_name);
        return -ENOENT;
    }
    uintptr_t address = probe.addr;
    unregister_kprobe(&probe);
    pr_info("Resolved symbol %s at address: %pK\n", func_name, (void *)address);
    return address;
}

uintptr_t resolve_kernel_symbol(const char *symbol_name) {
    if (!resolve_symbol_func) {
        resolve_symbol_func = get_symbol_address("kallsyms_lookup_name");
    }
    uintptr_t address = resolve_symbol_func(symbol_name);
    pr_info("Resolved kernel symbol %s at address: %pK\n", symbol_name, (void *)address);
    return address;
}

#endif