#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

/* Constant for the max semaphores */
#define MAX_SEMAPHORES 255

/* Define struct for semaphores */
struct semaphore_t {
	int init_value;
	int value;
	struct spinlock lock;
	int closed; // 0 if false and 1 if true
};

/* Define array of semaphores */
struct semaphore_t semaphore_array[MAX_SEMAPHORES];

/* Function to check if the semaphore ID is valid */
static int
invalid_id(int id)
{
  return id < 0 || id >= MAX_SEMAPHORES;
}

/* Initialize semaphores, all closed and with value 0 */
void
init_semaphores(void) 
{
  for (int i = 0; i < MAX_SEMAPHORES; i++) {
    semaphore_array[i].value = 0;
	semaphore_array[i].closed = 1;
  }
}

/* open a semaphore */
uint64
sys_sem_open(void)
{
	int value, sem_id;
	argint(0, &sem_id);
	argint(1, &value);
	if(invalid_id(sem_id) || value < 0) {
		/* return 0 if ID or the intial value is invalid */
		return 0;
	}
	if(semaphore_array[sem_id].closed == 0) {
		/* return -1 if its trying to open a semaphore that is already on use */
		return -1;
	}
	// Init semaphore_array lock
	initlock(&semaphore_array[sem_id].lock, "semaphore");
	semaphore_array[sem_id].value = value;
	semaphore_array[sem_id].init_value = value;
	semaphore_array[sem_id].closed = 0;
	return 1;
}

/* Increment semaphore */
uint64
sys_sem_up(void)
{
	int sem_id;
	argint(0, &sem_id);
	if(invalid_id(sem_id)) {
		return 0;
	}
	acquire(&semaphore_array[sem_id].lock);
	semaphore_array[sem_id].value++;
	if(semaphore_array[sem_id].closed == 1) {
		/* Return 0 if the semaphore_array is closed */
    	release(&semaphore_array[sem_id].lock);
		return 0;
	}
	if(semaphore_array[sem_id].value >= semaphore_array[sem_id].init_value) {
		/* Return -1 if it makes the adition past the initial value */
		wakeup(&semaphore_array[sem_id]);
		release(&semaphore_array[sem_id].lock);
		return -1;
	}
	wakeup(&semaphore_array[sem_id]);
	release(&semaphore_array[sem_id].lock);
  	return 1;
}

/* Decrement semaphore */
uint64
sys_sem_down(void)
{
	int sem_id;
	argint(0, &sem_id);
	if(invalid_id(sem_id)) {
		return 0;
	}
	acquire(&semaphore_array[sem_id].lock);
	if(semaphore_array[sem_id].closed == 1) {
    release(&semaphore_array[sem_id].lock);
		return 0;
	}
	while(semaphore_array[sem_id].value == 0) {
		/* Sleep the semaphore when the value is 0 */
		sleep(&semaphore_array[sem_id], &semaphore_array[sem_id].lock);
	}
	if(semaphore_array[sem_id].value > 0) {
		semaphore_array[sem_id].value--;
	}
	release(&semaphore_array[sem_id].lock);
	return 1;
}

/* Close semaphore */
uint64
sys_sem_close(void)
{
	int sem_id;
	argint(0, &sem_id);
	if(invalid_id(sem_id)) {
		return 0;
	}
	acquire(&semaphore_array[sem_id].lock);
	/* Close the semaphore when sem_close(sem_id) is called */
	if (semaphore_array[sem_id].closed == 0) {
		/* if the semaphore is open */
		semaphore_array[sem_id].value = 0;
		semaphore_array[sem_id].closed = 1;
	}
	/* jump here if the semaphore is already closed */
	release(&semaphore_array[sem_id].lock);
	return 1;
}