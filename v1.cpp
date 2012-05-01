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
		// append v to buffer
		while(buffer.size() == BUFFER_SIZE) {
			sleep(1);
		}
		buffer.push_back(v);
		cout << "Producer produced " << v << endl;
		sleep(1);
	}	
}

int take() {
	int x = -1;
	while (buffer.empty() || x <= 0) {
		x = buffer.front();
		sleep(2);
	}
	buffer.pop_front();
	return x;
}

void *Consumer(void *id) {
	long tid;
	tid = (long) id;
	while(true) {
		int a = 0;
		int x = take();
		printf("Consumer %ld consumed %i\n",tid, x);
		sleep(1);
	}
}

int main() {

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
      		if (rc){
         		printf("ERROR; return code from pthread_create() is %d\n", rc);
         		return -1;
 		}
	}

	pthread_exit(NULL);
}
