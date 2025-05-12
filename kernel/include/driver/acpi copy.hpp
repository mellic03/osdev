#pragma once

#include <kernel/boot_limine.hpp>
#include <stdint.h>
#include <stdbool.h>
#include <kernel/memory/vmm.hpp>

struct rsdp_t
{
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
} __attribute__((packed));

struct xsdp_t
{
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdtAddress;
   
    uint32_t length;
    uint64_t xsdtAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct acpi_sdt_hdr_t
{
    char sign[4];
    uint32_t length;
    uint8_t rev;
    uint8_t chksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_rev;
    uint32_t creator_id;
    uint32_t creator_rev;
} __attribute__((packed));

struct acpi_sdt_t
{
    acpi_sdt_hdr_t hdr;
    uint8_t data[];
} __attribute__((packed));

struct acpi_gas_t
{
    uint8_t addr_space_id;
    uint8_t reg_bit_width;
    uint8_t reg_bit_offset;
    uint8_t reserved;
    uint64_t address;
} __attribute__((packed));

void acpi_init(struct limine_rsdp_response *);
acpi_sdt_t *acpi_get_sdt(const char *sign);

