/** @file gettid.c
 *  @brief This file contains the definition for the sys_gettid() system call.
 *  @author akanjani, lramire1
 */

#include <kernel_state.h>

int sys_gettid() {
  return kernel.current_thread->tid;
}