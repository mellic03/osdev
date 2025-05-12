#include <kernel/module.hpp>
#include <kernel/log.hpp>
#include <sym/sym_t.hpp>
#include <filesystem/ustar.hpp>
#include <kernel/kstring.h>
#include <kmalloc.h>

#include <kernel/elf.h>


static const char *Elf64_etypeStr( uint32_t );
static const char *Elf64_ptypeStr( uint32_t );
static const char *Elf64_shtypeStr( uint32_t );



static void loadPhdrs( Elf64_Phdr *phdrs, size_t phnum )
{
    syslog log("loadPhdrs");
    for (uint32_t i=0; i<phnum; i++)
    {
        const auto &phdr = phdrs[i];
        log("p_type:   %s",    Elf64_ptypeStr(phdr.p_type));
        log("p_flags:  %lu",   phdr.p_flags);
        log("p_offset: 0x%lx", phdr.p_offset);
        log("p_vaddr:  %lu",   phdr.p_vaddr);
        log("p_paddr:  %lu",   phdr.p_paddr);
        log("p_filesz: %lu",   phdr.p_filesz);
        log("p_memsz:  %lu",   phdr.p_memsz);
        log("p_align:  %lu",   phdr.p_align);
        log("");
    }

}


static void loadSym( uint8_t *, const char *strtab, Elf64_Sym *sym )
{
    syslog log("loadSym");
    auto *name = strtab + sym->st_name;
    log("   name:   \"%s\"", name);
    log("st_info:   %lu",    sym->st_info);
    log("st_other:  %lu",    sym->st_other);
    log("st_shndx:  %lu",    sym->st_shndx);
    log("st_value:  %lu",    sym->st_value);
    log("st_size:   %lu",    sym->st_size);
}

static void loadRela( Elf64_Rela *relas, size_t count )
{
    syslog log("loadRelas");
    for (uint32_t i=0; i<count; i++)
    {
        auto &rela = relas[i];
        log("r_offset:  %lu", rela.r_offset);
        log("r_info:    %lu", rela.r_info);
        log("r_sym:     %lu", ELF64_R_SYM(rela.r_info));
        log("r_type:    %lu", ELF64_R_TYPE(rela.r_info));
        log("r_addend:  %lu", rela.r_addend);
        log("");
    }
}

static void loadShdrs( uint8_t *base, Elf64_Shdr *shdrs, size_t shnum )
{
    syslog log("loadShdrs");

    const char *strtab = nullptr;
    for (uint32_t i=0; i<shnum; i++)
    {
        if (shdrs[i].sh_type == SHT_STRTAB)
            strtab = (const char*)(base + shdrs[i].sh_offset);
    }

    for (uint32_t i=0; i<shnum; i++)
    {
        const Elf64_Shdr &shdr = shdrs[i];
        auto *name = strtab + shdr.sh_name;

        log("   name:     \"%s\"",    name);
        log("sh_type:      %s",    Elf64_shtypeStr(shdr.sh_type));
        log("sh_flags:     %lu",   shdr.sh_flags);
        log("sh_addr:      0x%lx", shdr.sh_addr);
        log("sh_offset:    %lu",   shdr.sh_offset);
        log("sh_size:      %lu",   shdr.sh_size);
        log("sh_link:      %lu",   shdr.sh_link);
        log("sh_info:      %lu",   shdr.sh_info);
        log("sh_addralign: %lu",   shdr.sh_addralign);
        log("sh_entsize:   %lu",   shdr.sh_entsize);
    
        if (strcmp(name, ".bss") == 0)
        {
            uint8_t *bss_base = base + shdr.sh_offset;
            memset(bss_base, 0, shdr.sh_size);
        }

        if (shdr.sh_type == SHT_DYNSYM)
        {
            loadSym(base, strtab, (Elf64_Sym*)(base + shdr.sh_offset));
        }

        if (shdr.sh_type == SHT_RELA)
        {
            size_t count = shdr.sh_size / shdr.sh_entsize;
            loadRela((Elf64_Rela*)(base + shdr.sh_offset), count);
        }

        log("");
    }

}


void loadElf64( void *data, size_t size )
{
    syslog log("kernel::loadModuleElf");

    void *addr = kmalloc(size);
    memcpy(addr, data, size);

    uint8_t    *base = (uint8_t*)addr;
    Elf64_Ehdr *ehdr = (Elf64_Ehdr*)base;
    log("e_type:        %s",    Elf64_etypeStr(ehdr->e_type));
    log("e_machine:     %lu", ehdr->e_machine);
    log("e_version:     %lu", ehdr->e_version);
    log("e_entry:       0x%lx", ehdr->e_entry);
    log("e_phoff:       %lu", ehdr->e_phoff);
    log("e_shoff:       %lu", ehdr->e_shoff);
    log("e_flags:       %lu", ehdr->e_flags);
    log("e_ehsize:      %lu", ehdr->e_ehsize);
    log("e_phentsize:   %lu", ehdr->e_phentsize);
    log("e_phnum:       %lu", ehdr->e_phnum);
    log("e_shentsize:   %lu", ehdr->e_shentsize);
    log("e_shnum:       %lu", ehdr->e_shnum);
    log("e_shstrndx:    %lu", ehdr->e_shstrndx);
    log("");

    loadPhdrs((Elf64_Phdr*)(base + ehdr->e_phoff), ehdr->e_phnum);
    loadShdrs(base, (Elf64_Shdr*)(base + ehdr->e_shoff), ehdr->e_shnum);
}











static const char *Elf64_etypeStr( uint32_t etype )
{
    switch (etype)
    {
        default: return "INVALID";
        case ET_NONE: return "ET_NONE";
        case ET_REL: return "ET_REL";
        case ET_EXEC: return "ET_EXEC";
        case ET_DYN: return "ET_DYN";
        case ET_CORE: return "ET_CORE";
        case ET_NUM: return "ET_NUM";
    }
}

static const char *Elf64_ptypeStr( uint32_t ptype )
{
    switch (ptype)
    {
        default: return "INVALID";
        case PT_NULL: return "PT_NULL";
        case PT_LOAD: return "PT_LOAD";
        case PT_DYNAMIC: return "PT_DYNAMIC";
        case PT_INTERP: return "PT_INTERP";
        case PT_NOTE: return "PT_NOTE";
        case PT_SHLIB: return "PT_SHLIB";
        case PT_PHDR: return "PT_PHDR";
        case PT_TLS: return "PT_TLS";
        case PT_NUM: return "PT_NUM";
        case PT_LOOS: return "PT_LOOS";
    }
}

static const char *Elf64_shtypeStr( uint32_t shtype )
{
    switch (shtype)
    {
        default: return "INVALID";
        case SHT_NULL: return "SHT_NULL";
        case SHT_PROGBITS: return "SHT_PROGBITS";
        case SHT_SYMTAB: return "SHT_SYMTAB";
        case SHT_STRTAB: return "SHT_STRTAB";
        case SHT_RELA: return "SHT_RELA";
        case SHT_HASH: return "SHT_HASH";
        case SHT_DYNAMIC: return "SHT_DYNAMIC";
        case SHT_NOTE: return "SHT_NOTE";
        case SHT_NOBITS: return "SHT_NOBITS";
        case SHT_REL: return "SHT_REL";
        case SHT_SHLIB: return "SHT_SHLIB";
        case SHT_DYNSYM: return "SHT_DYNSYM";
        case SHT_INIT_ARRAY: return "SHT_INIT_ARRAY";
        case SHT_FINI_ARRAY: return "SHT_FINI_ARRAY";
        case SHT_PREINIT_ARRAY: return "SHT_PREINIT_ARRAY";
        case SHT_GROUP: return "SHT_GROUP";
        case SHT_SYMTAB_SHNDX: return "SHT_SYMTAB_SHNDX";
        case SHT_RELR: return "SHT_RELR";
        case SHT_NUM: return "SHT_NUM";
    }
}


