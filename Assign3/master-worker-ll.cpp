#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include <iostream>

int count=0;
int item_to_produce=0;
int total_items, max_buf_size, num_workers;

// Declaration of thread condition variable 
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER; //not empty anymore
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER; //not full anynore

// declaring mutex 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

pthread_t* tid;
int* buf;
int* ptr;
int* worker_thread_id;
int loc=0, j, l;
// declare any global data structures, variables, etc that are required
// e.g buffer to store items, pthread variables, etc

void print_produced(int num) {

  printf("Produced %d\n", num);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}


/* produce items and place in buffer (array or linked list)
 * add more code here to add items to the buffer (these items will be consumed
 * by worker threads)
 * use locks and condvars suitably
 */
void *generate_requests_loop(void *data)
{
  
  int thread_id = *((int *)data);

  while(1){ 
    if(item_to_produce >= total_items)
	    break;
    pthread_mutex_lock(&lock);
    while (loc==max_buf_size){
        pthread_cond_wait(&cond2, &lock);
    } 
    ptr=(int*)realloc(ptr, (loc+1)*sizeof(int)); //increasing allocated memory
    ptr[loc]=item_to_produce; 
    loc++;
    
    print_produced(item_to_produce);
    item_to_produce++;
    pthread_cond_broadcast(&cond1);
    pthread_mutex_unlock(&lock);
  }
  pthread_cond_broadcast(&cond1);
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item
void *doSomeThing(void *arg){

  
   while(1){ 

    if(item_to_produce >= total_items && loc==0)
      break;
    pthread_mutex_lock(&lock);
    int thread_id = *((int *)arg);
    while (loc==0){
      if(item_to_produce >= total_items && loc==0){
        pthread_mutex_unlock(&lock);
        return NULL;
      }
      pthread_cond_wait(&cond1, &lock);
      }
  loc--;
  print_consumed(ptr[loc], thread_id);
  ptr=(int*)realloc(ptr, (loc)*sizeof(int)); //freeing memory, keeping size as much is required
  pthread_cond_broadcast(&cond2);
  pthread_mutex_unlock(&lock);
  }
    pthread_cond_broadcast(&cond1);
    return NULL;
}

int main(int argc, char *argv[])
{
 
  int master_thread_id = 0;
  pthread_t master_thread;
  item_to_produce = 0;
  
  if (argc < 4) {
    printf("./master-worker #total_items #max_buf_size #num_workers e.g. ./exe 10000 1000 4\n");
    exit(1);
  }
  else {
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
  
  // Initlization code for any data structures, variables, etc
  int i = 0;
  int err;
  buf=new int [max_buf_size];
  worker_thread_id=new int [num_workers];
  tid= new pthread_t [num_workers];

  if (pthread_mutex_init(&lock, NULL) != 0){
    printf("\n mutex init failed\n");
    return 1;
  }


  //create master producer thread
  pthread_create(&master_thread, NULL, generate_requests_loop, (void *)&master_thread_id);

  //create worker consumer threads
  while(i < num_workers){
    worker_thread_id[i]=i+1;
    err = pthread_create(&(tid[i]), NULL, &doSomeThing, (void *)&worker_thread_id[i]);
    if (err != 0)
      printf("\ncan't create thread :[%s]", strerror(err));
    i++;
    }  
  

  //wait for all threads to complete

  for (int k=0; k<num_workers; k++ ){
    pthread_join(tid[k], NULL);
  }
  pthread_join(master_thread, NULL);
  printf("master joined\n");
  

  pthread_mutex_destroy(&lock);
  //deallocate and free up any memory you allocated
  free(ptr);
  
  return 0;
}
