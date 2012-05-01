/*
  Author: Brendan Shaklovitz
*/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <iostream>
#include <deque>
#include <unistd.h>
#define NUM_CONSUMERS	5
#define BUFFER_SIZE	5
using namespace std;

deque <int> buffer;
sem_t e; // producer waits
sem_t n; // consumer waits
sem_t c; // access to buffer control
int pIndex = 0;
int produced = 1;
int cIndex = 0;

int produce() {
	int p = produced;
	produced++;
	return p;
}

void *Producer(void *what) {
	while (true) {
		int v =	produce();
		sem_wait(&e);
		// append v to buffer
		buffer.push_back(v);
		cout << "Producer produced " << v << endl;
		sem_post(&n);
		sleep(1);
	}	
}

int take() {
	int x = buffer.front();
	buffer.pop_front();
	return x;
}

void *Consumer(void *id) {
	long tid;
	tid = (long)id;
	while(true) {
		sem_wait(&n);
		sem_wait(&c);
		int x = take();
		sem_post(&c);
		sem_post(&e);
		printf("Consumer %ld consumed %i\n",tid, x);
		sleep(1);
	}
}

int main () {
	// create / initialize semaphores
	// e (max), n 0, c 1
	sem_init(&e, 0, BUFFER_SIZE);
	sem_init(&n, 0, 0);
	sem_init(&c, 0, 1);

	pthread_t producer;
   	pthread_t consumers[NUM_CONSUMERS];
   	int rc;
   	long t;
	rc = pthread_create(&producer, NULL, Producer, NULL);
	if (rc) {
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		return -1;
	}
   	for(t = 0; t < NUM_CONSUMERS; t++) {
	  	//printf("In main: creating thread %ld\n", t);
	  	rc = pthread_create(&consumers[t], NULL, Consumer, (void *)t);
	  	if (rc) {
		 	printf("ERROR; return code from pthread_create() is %d\n", rc);
		 	return -1;
 		}
	}

	pthread_exit(NULL);
}
