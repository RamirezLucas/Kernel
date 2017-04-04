/** @file kernel_state.h
 *  @brief This file contains the declaration for the kernel_state_t data
 *  structure and the functions to act on it.
 *  @author akanjani, lramire1
 */

#ifndef _KERNEL_STATE_H_
#define _KERNEL_STATE_H_

#include <hash_table.h>
#include <mutex.h>
#include <static_queue.h>
#include <tcb.h>

#define KERNEL_INIT_FALSE 0
#define KERNEL_INIT_TRUE 1
#define CPU_IDLE_FALSE 0
#define CPU_IDLE_TRUE 1
#define FIRST_TASK "coolness"

typedef struct kernel {

  /** @brief Indicate whether the kernel state is initialized or not */
  char init;

  /* @brief Holds the TCB of the thread currently running,
   * only works for kernel running on uniprocessor */
  tcb_t *current_thread;

  /* @brief Hold the task id that should be assigned to the next
   * task created, the value is incremented each time a task is created */
  int task_id;

  /* @brief Hold the thread id that should be assigned to the next
   * thread created, the value is incremented each time a thread is created */
  int thread_id;

  /* @brief Queue containing the list of runnable threads, in the order
   * that they should be run in. The queue should be initialized by calling
   * kernel_init()  */
  static_queue_t runnable_threads;

  /* @brief Idle thread (ran when there is nothing to run) */
  tcb_t *idle_thread;

  /* @brief Indicates wether the CPU is currently running the idle thread */
  int cpu_idle;

  /** @brief Mutex used to ensure atomicity when changing the kernel state */
  mutex_t mutex;

  /* ------------------------- */

  /** @brief Hash table holding all the PCBs */
  generic_hash_table_t pcbs;

  /** @brief Hash table holding all the TCBs */
  generic_hash_table_t tcbs;

} kernel_t;

/* Hold the kernel state*/
kernel_t kernel;

int kernel_init();
pcb_t *create_new_pcb();
tcb_t *create_new_tcb(const pcb_t *pcb, uint32_t esp0, uint32_t cr3);

tcb_t* create_idle_thread();

unsigned int hash_function_pcb(void *pcb, unsigned int nb_buckets);
int find_pcb(void *pcb, void *tid);
unsigned int hash_function_tcb(void *pcb, unsigned int nb_buckets);
int find_tcb(void *pcb, void *tid);

#endif /* _KERNEL_STATE_H_ */
