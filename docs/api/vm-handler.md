# VM Handler API Reference

## Overview

The VM Handler API provides interfaces for managing virtual machine lifecycle and operations.

## Core Functions

### VM Creation

```c
int create_vm(vm_config* config, uint64_t* vm_id);
```

**Parameters:**
- `config`: Configuration structure for the new VM
- `vm_id`: Pointer to store the ID of created VM

**Returns:**
- 0 on success
- Error code on failure

### VM Destruction

```c
int destroy_vm(uint64_t vm_id);
```

**Parameters:**
- `vm_id`: ID of the VM to destroy

**Returns:**
- 0 on success
- Error code on failure

### VM State Management

```c
int get_vm_state(uint64_t vm_id, vm_state* state);
int set_vm_state(uint64_t vm_id, vm_state new_state);
```

**Parameters:**
- `vm_id`: Target VM ID
- `state`: Current VM state
- `new_state`: Desired VM state

## Resource Management

### Memory Allocation

```c
int allocate_vm_memory(uint64_t vm_id, size_t size);
int deallocate_vm_memory(uint64_t vm_id);
```

### CPU Resource Management

```c
int assign_vcpu(uint64_t vm_id, uint32_t vcpu_count);
int configure_vcpu(uint64_t vm_id, vcpu_config* config);
```

## Error Handling

### Error Codes

```c
#define VM_SUCCESS          0
#define VM_ERR_INVALID_ID   1
#define VM_ERR_NO_RESOURCE  2
#define VM_ERR_INVALID_STATE 3
```

## Usage Examples

### Creating and Starting a VM

```c
vm_config config = {
    .memory_size = 1024 * 1024 * 1024,  // 1GB
    .vcpu_count = 2
};

uint64_t vm_id;
int result = create_vm(&config, &vm_id);
if (result == VM_SUCCESS) {
    result = set_vm_state(vm_id, VM_STATE_RUNNING);
}
```

## Best Practices

1. Always check return values for error conditions
2. Properly initialize configuration structures
3. Clean up resources when destroying VMs
4. Handle state transitions appropriately

## Performance Considerations

- Minimize VM creation/destruction cycles
- Optimize memory allocation patterns
- Consider CPU scheduling impact

## Thread Safety

All API functions are thread-safe unless otherwise noted.

## Version History

### v1.0
- Initial API release
- Basic VM lifecycle management
- Resource allocation functions