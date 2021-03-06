/** @file virtual_memory_helper.h
 *  @brief  This file contains the declarations for helper functions used to 
 *          access and/or modify the virtual memory system's state
 *  @author akanjani, lramire1
 */

#ifndef _VIRTUAL_MEMORY_HELPER_H_
#define _VIRTUAL_MEMORY_HELPER_H_

#include <stdint.h>

/* Misc */
int is_entry_present(unsigned int *entry_addr);
void set_entry_invalid(unsigned int *entry_addr, unsigned int address);
unsigned int *create_page_table(unsigned int *page_directory_entry_addr,
                                uint32_t flags, int is_first_task);
unsigned int *create_page_table_entry(unsigned int *page_table_entry_addr,
                                      uint32_t flags);

/* Getters */
unsigned int *get_page_table_addr(unsigned int *page_directory_entry_addr);
unsigned int *get_frame_addr(unsigned int *page_table_entry_addr);
unsigned int *get_page_dir_entry(unsigned int address);
unsigned int *get_page_table_entry(
                unsigned int *page_directory_entry_addr, unsigned int address);
uint32_t get_entry_flags(unsigned int *entry_addr);
unsigned int get_virtual_address(unsigned int *page_directory_entry_addr,
                                  unsigned int *page_table_entry_addr);

/* Frame allocation */
unsigned int* allocate_frame();
int free_frame(unsigned int* addr);

/* ZFOD related functions */
int mark_address_requested(unsigned int address);
int mark_address_range_requested(unsigned int address, unsigned int len);
int allocate_frame_if_address_requested(unsigned int address);
int is_page_requested(unsigned int *addr);


/** @brief Invalidates a page stored in the TCB
 *
 *  If the page indicated by the virtual address does not exist in the TLB,
 *  the function has no effect.
 *
 *  @param addr A virtual address in the range of virtual addresses of the page
 *    that should be invalidated
 *
 *  @return void
 */
void invalidate_tlb(unsigned int addr);

#endif /* _VIRTUAL_MEMORY_HELPER_H_ */
