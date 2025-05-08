#pragma once
#include <stddef.h>
#include <stdint.h>


// End-of-interfaces marker
static constexpr uint64_t ITABLE_BEGIN        = 0x00AA;
static constexpr uint64_t ITABLE_END          = 0xFF00;
static constexpr uint64_t ITABLE_BLOCK_DEVICE = 0x00A0;
static constexpr uint64_t ITABLE_CHAR_DEVICE  = 0x00A1;
static constexpr uint64_t ITABLE_XXXX_DEVICE  = 0x00A2;


struct iTableEntry
{
    uint64_t id;
    void *vtable;
};




struct BlockDeviceInterface
{
    using entryFn = int (*)(void*);
    using closeFn = int (*)(void);
    using readFn  = int (*)(uintptr_t, size_t, void*);
    using writeFn = int (*)(uintptr_t, size_t, const void*);

    entryFn  open;
    closeFn  close;
    readFn   read;
    writeFn  write;

    BlockDeviceInterface( entryFn*, closeFn*, readFn*, writeFn* );
};



struct CharDeviceInterface
{
    using entryFn = void (*)(void*);
    using closeFn = void (*)(void);
    using readFn  = size_t (*)(void*, size_t);
    using writeFn = size_t (*)(const void*, size_t);

    entryFn  entry;
    closeFn  close;
    readFn   read;
    writeFn  write;

    CharDeviceInterface( entryFn A, closeFn B, readFn C, writeFn D )
    : entry(A), close(B), read(C), write(D) {  };

    CharDeviceInterface()
    : CharDeviceInterface(nullptr, nullptr, nullptr, nullptr) {  };
};



