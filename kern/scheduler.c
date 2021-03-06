/** @file scheduler.c
 *  @brief  This file contains the definition for the functions related to
 *          thread scheduling
 *  @author akanjani, lramire1
 */

#include <asm.h>
#include <context_switch.h>
#include <kernel_state.h>
#include <scheduler.h>
#include <generic_node.h>
#include <stdlib.h>
#include <tcb.h>
#include <page.h>

#include <assert.h>
#include <simics.h>

/** @brief  Returns the next thread to run from the queue of runnable threads
 *
 *  If the queue of runnable threads is empty, then the function returns the
 *  TCB of the idle thread
 *
 *  @return The next thread's TCB
 */
tcb_t *next_thread() {

  // Check if the kernel state is initialized
  assert(kernel.current_thread != NULL && kernel.init == KERNEL_INIT_TRUE);

  generic_node_t *next_thread = stack_queue_dequeue(&kernel.runnable_queue);
  if (next_thread == NULL) {
    // If the queue is empty, run the idle thread
    return kernel.idle_thread;
  }

  return next_thread->value;

}

/** @brief  Enqueues the invoking thread in the queue of runnable threads and
 *          context switch to the next thread in the queue
 *
 *  If the invoking thread is the idle thread, then it is not added to the 
 *  queue of runnable threads. 
 *
 *  @return void
 */
void make_runnable_and_switch() {

  assert(kernel.current_thread != NULL && kernel.init == KERNEL_INIT_TRUE);

  disable_interrupts();

  kernel.current_thread->thread_state = THR_RUNNABLE;

  if (kernel.cpu_idle == CPU_IDLE_TRUE) {
    context_switch(next_thread());
    return;
  }

  generic_node_t new_tail = {kernel.current_thread, NULL};
  stack_queue_enqueue(&kernel.runnable_queue, &new_tail);

  context_switch(next_thread());
  
}

/** @brief  Blocks the invoking thread and context switches to another thread
 *
 *  @param  holding_mutex Indicates whether the function should unlock mutex
 *                        on the invoking thread's state
 *  @param  mp            A pointer to an eff_mutex to unlock depending on the
 *                        value of holding_mutex
 *
 *  @return void
 */
void block_and_switch(int holding_mutex, eff_mutex_t *mp) {

  assert(kernel.current_thread != NULL && kernel.init == KERNEL_INIT_TRUE);

  disable_interrupts();

  if (holding_mutex == HOLDING_MUTEX_TRUE) {
    eff_mutex_unlock(mp);
  }

  kernel.current_thread->thread_state = THR_BLOCKED;

  context_switch(next_thread());

}

/** @brief  Makes a thread runnable by adding it to the runnable queue
 *
 *  If the thread that we are trying to make runnable is already in the 
 *  THR_RUNNABLE state, then the function has no effect.
 *
 *  @param  tcb The TCB of the thread to make runnable
 *
 *  @return void
 */
void add_runnable_thread(tcb_t *tcb) {

  assert(tcb != NULL && kernel.init == KERNEL_INIT_TRUE);
  assert(tcb != kernel.idle_thread);

  disable_interrupts();

  // Reject the call if the thread is already in the runnable queue
  if (tcb->thread_state == THR_RUNNABLE) {
    enable_interrupts();
    return;
  }


  tcb->thread_state = THR_RUNNABLE;

  // Create node at the lowest address of the thread's kernel stack
  generic_node_t new_tail = {tcb, NULL};
  generic_node_t * node_addr = (generic_node_t *)(tcb->esp0 - PAGE_SIZE);
  *(node_addr) = new_tail;

  // Enqueue the thread
  stack_queue_enqueue(&kernel.runnable_queue, node_addr);

  enable_interrupts();

}

/** @brief  Makes a thread runnable by adding it to the runnable queue
 *
 *  If the thread that we are trying to make runnable is already in the 
 *  THR_RUNNABLE state, then the function has no effect.
 *  The only difference between this function and add_runnable_thread() is that
 *  this one does not disable/enable interrupts, hence it should only be called
 *  with interrupts already disabled
 *
 *  @param  tcb The TCB of the thread to make runnable
 *
 *  @return void
 */
void add_runnable_thread_noint(tcb_t *tcb) {

  assert(tcb != NULL && kernel.init == KERNEL_INIT_TRUE);
  assert(tcb != kernel.idle_thread);

  // Reject the call if the thread is already in the runnable queue
  if (tcb->thread_state == THR_RUNNABLE) {
    return;
  }

  tcb->thread_state = THR_RUNNABLE;

  // Create node at the lowest address of the thread's kernel stack
  generic_node_t new_tail = {tcb, NULL};
  generic_node_t * node_addr = (generic_node_t *)(tcb->esp0 - PAGE_SIZE);
  *(node_addr) = new_tail;

  // Enqueue the thread
  stack_queue_enqueue(&kernel.runnable_queue, node_addr);

}

/** @brief  Forces the kernel to run a particular thread
 *
 *  The function performs a context switch to a particular thread given as
 *  argument, overriding the scheduler's normal behavior. The invoking thread
 *  is put in the queue of runnable threads.
 *  
 *  @param  force_next_tcb The TCB of the thread to run next
 *
 *  @return 0 on success (in that case the context switch to the thread in
 *          argument happened), a negative number if the thread passed as an
 *          argument was not runnable at the moment of the call 
 */
int force_next_thread(tcb_t *force_next_tcb) {

  assert(kernel.current_thread != NULL && force_next_tcb != NULL &&
         kernel.init == KERNEL_INIT_TRUE);
  assert(force_next_tcb != kernel.idle_thread);

  disable_interrupts();    

  if (force_next_tcb->thread_state != THR_RUNNABLE) {
    enable_interrupts();
    return -1;
  }

  kernel.current_thread->thread_state = THR_RUNNABLE;

  // Create new node for runnable queue
  generic_node_t new_tail = {kernel.current_thread, NULL};

  // Enqueue the current thread
  stack_queue_enqueue(&kernel.runnable_queue, &new_tail);

  // Traverse the queue of runnable threads and delete any occurence of the
  // forced thread
  generic_node_t *it = kernel.runnable_queue.head, *prev = NULL;
  while (it != NULL) {
    if (it->value == force_next_tcb) {
      if (prev == NULL) {
        kernel.runnable_queue.head = it->next;
      } else {
        prev->next = it->next;
      }
    }
    prev = it;
    it = it->next;
  }

  context_switch(force_next_tcb);

  return 0;

}
