#pragma once
#include <stddef.h>
#include <stdint.h>


// Interface table structure
struct itable
{
    uint64_t id;    // Unique identifier for the interface
    void *vtable;   // Pointer to the interface's virtual table
};

// End-of-interfaces marker
#define END_OF_INTERFACES 0

// Function prototype for searching the interface table by ID
void* search_itable_for_id(int id);



// Block device interface ID
#define BlockDeviceInterface 0xb10c

using bdOpenFn  = int (*)(void*);
using bdCloseFn = int (*)(void);
using bdReadFn  = int (*)(uintptr_t, size_t, void*);
using bdWriteFn = int (*)(uintptr_t, size_t, const void*);

struct BlockDeviceInterface
{
    bdOpenFn  open;
    bdCloseFn close;
    bdReadFn  read;
    bdWriteFn write;
};




// Block device interface ID
#define CharDeviceInterface 0xb10d

using cdEntryFn = void (*)(void*);
using cdCloseFn = int (*)(void);
using cdReadFn  = size_t (*)(void*, size_t);
using cdWriteFn = size_t (*)(const void*, size_t);

struct CharDeviceInterface
{
    cdEntryFn entry;
    cdCloseFn close;
    cdReadFn  read;
    cdWriteFn write;
};



