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
int load_every_segment(const simple_elf_t *elf);
int load_segment(const char *fname, unsigned long offset, unsigned long size,
                 unsigned long start_addr, int type);
void *load_frame(unsigned int address, unsigned int type);
int load_multiple_frames(unsigned int address, unsigned int nb_frames, 
                            unsigned int type);
int free_address_space(unsigned int *page_table_addr, int free_kernel_space);
int free_page_table(unsigned int *page_table_addr, int free_kernel_space);
void free_frames_range(unsigned int *start_dir_entry, 
                      unsigned int *start_table_entry,
                      unsigned int *end_dir_entry,
                      unsigned int *end_table_entry);

int is_valid_string(char *addr);

void vm_enable();

#endif /* _VIRTUAL_MEMORY_H_ */
