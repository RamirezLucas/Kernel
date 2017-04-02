/** @file virtual_memory.h
 *  @brief This file contains the declarations for functions manipulating
 *  virtual memory
 *  @author akanjani, lramire1
 */

#ifndef _VIRTUAL_MEMORY_H_
#define _VIRTUAL_MEMORY_H_

#include <elf_410.h>

int vm_init();
unsigned int *setup_vm(const simple_elf_t *elf);
int load_every_segment(const simple_elf_t *elf, unsigned int *cr3);
int load_segment(const char *fname, unsigned long offset, unsigned long size,
                 unsigned long start_addr, int type, unsigned int *cr3);
void *load_frame(unsigned int address, unsigned int type, unsigned int *base);
int free_address_space(unsigned int *page_table_addr, int free_kernel_space);
int free_page_table(unsigned int *page_table_addr, int free_kernel_space);

void vm_enable();

unsigned int *get_page_directory_addr(unsigned int *address, unsigned int *base);

int is_valid_string(char *addr);
unsigned int *get_page_table_addr_with_offset(unsigned int *page_directory_entry_addr, unsigned int address);
#endif /* _VIRTUAL_MEMORY_H_ */
