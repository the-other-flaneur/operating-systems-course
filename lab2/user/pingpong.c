#include "kernel/syscall.h"
#include "kernel/types.h"
#include "user/user.h"

/* struct to hold ID's of the semaphores for this pingpong process */
typedef struct {
  int parent;
  int child;
} sem_id;

/* print ping */
static 
void
ping(int n, int parent, int child)
{
  for(int i = 0; i < n; i++) {
    if(sem_down(child)==0) {
      printf("ERROR: on sem down\n");
      exit(1);
    }
    printf("Ping\n");
    if(sem_up(parent)==0){
      printf("ERROR: on sem up\n");
      exit(1);
    }
  }
}

/* print pong */
static 
void
pong(int n, int parent, int child)
{
  for(int i = 0; i < n; i++) {
    if(sem_down(parent)==0) {
      printf("ERROR on sem down\n");
      exit(1);
    }
    printf("Pong\n");
    if(sem_up(child)==0){
      printf("ERROR on sem up\n");
      exit(1);
    }
  }
}

/* checks the semaphore_array for closed semaphores to use */
static
sem_id init_ids(sem_id sem_ids)
{
  int status = -1;
  while(status == -1) {
    status = sem_open(sem_ids.parent, 0);
    if(status == 0) {
      printf("ERROR: sem open\n");  
      exit(1);
    } else if(status == -1) {
      sem_ids.parent++;
    }
  }
  status = -1;
  while(status == -1) {
    status = sem_open(sem_ids.child, 1);
    if(status == 0) {
      printf("ERROR: sem open\n");  
      exit(1);
    } else if(status == -1) {
      sem_ids.child++;
    }
  }
  return sem_ids;
}

/* PingPong */
int 
main(int argc, char **argv) 
{
  sem_id sem_ids;
  sem_ids.parent = 0;
  sem_ids.child = 1;
  sem_ids = init_ids(sem_ids);

	int n = atoi(argv[1]);
  if(n <= 0 || argc != 2) {
    printf("ERROR: usage: pingpong <rally>\n");
    exit(1);
  }

	/* Create a child process and run ping or pong for the parent and the child */
	int pid = fork();
  if(pid == -1) {
    printf("ERROR: on fork\n");
    return 0;
  } else if(pid == 0) {
    ping(n, sem_ids.parent, sem_ids.child);
    exit(0);
	} else {
    pong(n, sem_ids.parent, sem_ids.child);
    wait(0);
	}

  /* close semaphores after use */
  if(sem_close(sem_ids.parent)==0) {
    printf("ERROR: sem close\n");
    exit(1);
  }
  if(sem_close(sem_ids.child)==0) {
    printf("ERROR: sem close\n");
    exit(1);
  }
	exit(0);
}