/** @file syscalls.h
 *  @brief This file contains the declarations for the system calls handler
 *  functions.
 *  @author akanjani, lramire1
 */

#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <tcb.h>

int kern_gettid();

/* Scheduling related calls */
int kern_yield(int tid);
int kern_deschedule(int *reject);
int kern_make_runnable(int tid);

/* Forking calls */
int thread_fork();
int kern_fork(unsigned int * esp);
int kern_thread_fork(unsigned int * esp);

/* New/Remove pages calls */
int kern_new_pages(void *base, int len);
int kern_remove_pages(void *base);

/* Console IO */
int readline(int len, char *buf);
int print(int len, char *buf);

/* Exec calls */
int kern_exec(char *execname, char **argvec);
char *load_args_for_new_program(char **argvec, unsigned int *new_ptd, int count);

/* swexn calls */
int swexn(void *esp3, swexn_handler_t eip, void *arg, ureg_t *newureg);
int kern_swexn(void *esp3, swexn_handler_t eip, void *arg, ureg_t *newureg,
               unsigned int * kern_esp);

/* wait call */
int kern_wait(int *status_ptr);

/* Vanish call */
void kern_vanish(void);

/* Sleep */
int kern_sleep(int ticks);
void wake_up_threads(unsigned int ticks);

/* Set status */
void kern_set_status(int status);

/* Get ticks*/
unsigned int kern_get_ticks();

/* Helper function */
char *get_esp();
int create_stack_sw_exception(unsigned int cause, char *stack_ptr);
#endif /* _SYSCALLS_H_ */
