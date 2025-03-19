
# NAME ?= main

# CFLAGS := -std=c11 \
#         -O0 \
#         -I $(LIBDIR)/include/ \
#         -ffreestanding \
#         -Wall \
#         -Wextra \
#         -Werror=return-type \
#         -mgeneral-regs-only \
#         -fno-asynchronous-unwind-tables \
#         -fno-exceptions \
#         -fno-stack-protector \
#         -fno-stack-check \
#         -fno-PIC \
#         -mno-80387 \
#         -z max-page-size=0x1000 -mno-red-zone \
#         -m64 -mcmodel=large -march=x86-64 \
#         -Wno-missing-field-initializers \
#         -mno-mmx -mno-sse -mno-sse2 \
#         -nostdlib

# LIBSTIM_DIR := ../lib/libstim/
# INCLUDE_DIRS = $(LIBSTIM_DIR)/include/

# ASM_SOURCES := $(wildcard ../lib/libstim/src/*.asm)
# C_SOURCES   := $(wildcard ./src/$(NAME).c)   $(wildcard ../lib/libstim/src/*.c)
# OBJECTS     := $(C_SOURCES:.c=.c.o) $(ASM_SOURCES:.asm=.asm.o)


# $(NAME).elf: $(OBJECTS)
# 	x86_64-elf-ld -o $@ $^ && objcopy -O binary -j .text $(NAME).elf $(NAME).exec


# %.c.o: %.c
# 	x86_64-elf-gcc -I $(INCLUDE_DIRS) $(CFLAGS) -c $< -o $@

# %.asm.o: %.asm
# 	nasm -felf64 $< -o $@

# # .PHONY: clean
# # clean:
# # 	rm -f $(OBJECTS) program.elf
