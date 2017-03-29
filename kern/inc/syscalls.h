/** @file syscalls.h
 *  @brief This file contains the declarations for the system calls handler
 *  functions.
 *  @author akanjani, lramire1
 */

#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

int kern_gettid();

/* Scheduling related calls */
int kern_yield(int tid);
int kern_deschedule(int *reject);
int kern_make_runnable(int tid);

/* Forking calls */
int kern_fork(unsigned int * esp);
int kern_thread_fork(void);

int kern_exec(char *execname, char **argvec);
char *load_args_for_new_program(char **argvec, unsigned int *new_ptd, int count);
#endif /* _SYSCALLS_H_ */
