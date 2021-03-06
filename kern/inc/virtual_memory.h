/** @file virtual_memory.h
 *  @brief This file contains the declarations for functions manipulating
 *  virtual memory
 *  @author akanjani, lramire1
 */

#ifndef _VIRTUAL_MEMORY_H_
#define _VIRTUAL_MEMORY_H_

#include <elf_410.h>

/* Loading memory */
int load_every_segment(const simple_elf_t *elf, unsigned int *cr3);
int load_segment(const char *fname, unsigned long offset, unsigned long size,
                 unsigned long start_addr, int type, unsigned int *cr3);
void *load_frame(unsigned int address, unsigned int type, unsigned int *cr3,
                 int is_first_task);

/* Freeing memory */                 
int free_address_space(unsigned int *page_table_addr, int free_kernel_space);
int free_page_table(unsigned int * page_dir_entry_addr, 
                    unsigned int *page_table_addr, int free_kernel_space);
void free_frames_range(unsigned int address, unsigned int nb_frames);

/* Memory checking */
int is_buffer_valid(unsigned int address, int len, int read_only);
int is_valid_string(char *addr);

/* Enabling VM */
int vm_init();
unsigned int *setup_vm(const simple_elf_t *elf, int is_first_task);
void vm_enable();


#endif /* _VIRTUAL_MEMORY_H_ */
