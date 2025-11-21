
#ifndef SYSSEMAPHORE_H
#define SYSSEMAPHORE_H

/* Initialize semaphores all closed and with value 0 */
void
init_semaphores(void);

/* Increment semaphore */
uint64
sys_sem_up(void);

/* Decrement semaphore */
uint64
sys_sem_down(void);

/* Close semaphore */
uint64
sys_sem_close(void);

#endif // SYSSEMAPHORE_H
