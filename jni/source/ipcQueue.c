#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <pthread.h>
#include <stdlib.h>
#include <strings.h>
#include "errors.h"
#include "ipcQueue.h"

#define MAX_QUEUE_SIZE 512

struct queue {
	int rear;
	int front;
	PacketInfo pkts[MAX_QUEUE_SIZE];
	pthread_mutex_t mutex;
};


queue_t
createQueue()
{
	struct queue *q = (struct queue *) MALLOC(sizeof(struct queue));
	if (q == NULL) {
		printf("\nUnable to allocate Queue");
		return NULL;
	}

	q->front = -1;
	q->rear = -1;
	bzero(q->pkts, sizeof(q->pkts));

	int retval;
	char errstr[BUFSIZ];
	retval = pthread_mutex_init(&q->mutex, NULL);
	if (retval != 0) {
		strerror_r(retval, errstr, BUFSIZ);
		d_warning("init mutex failed %d: %s", retval, errstr);
		FREE(q);
		return NULL;
	}	  

	return (queue_t) q;
}

int
sendQueue(queue_t eq, PacketInfo pkt)
{
	struct queue *q = (struct queue *) eq;

	pthread_mutex_lock(&q->mutex);

	if (((q->front == 0) && (q->rear == MAX_QUEUE_SIZE-1)) 
		|| (q->front == q->rear+1)) { 

		pthread_mutex_unlock(&q->mutex);
		return -1;
	}

	if (q->front == -1) {
		q->front = q->rear = 0 ;
		q->pkts[q->rear] = pkt;
	} else {
		if (q->rear > (MAX_QUEUE_SIZE -1)) q->rear = 0;
		else q->rear++;

		q->pkts[q->rear] = pkt;
	}
 
	pthread_mutex_unlock(&q->mutex);

	return 0;
}

int
receiveQueue(queue_t eq, PacketInfo * pkt)
{
	struct queue *q = (struct queue *) eq;

	pthread_mutex_lock(&q->mutex);

	if (q->front == -1) {
		pthread_mutex_unlock(&q->mutex);
		return -1;
	}  

	*pkt = q->pkts[q->front];
	
	if(q->front == q->rear)
		q->front = q->rear = -1;
	else {
		if(q->front == (MAX_QUEUE_SIZE -1))
			q->front = 0;
		else
			q->front++;
	}

	pthread_mutex_unlock(&q->mutex);

	return 0;
}

/* Destroy the queue and free the elements which might be avaliable inside the queue.*/
void 
destroyQueue(queue_t eq)
{
	struct queue *q = (struct queue *) eq;
	
	//This will clean up queue element and free the message buffers
	PacketInfo pkt;
	while(receiveQueue(eq, &pkt) == 0) {
		FREE(pkt.msgPtr);
	}
	int retval;
	char errstr[BUFSIZ];
	retval = pthread_mutex_destroy(&q->mutex);
	if (retval != 0) {
		strerror_r(retval, errstr, BUFSIZ);
		d_warning("Destroy mutex failed %d: %s\n", retval, errstr);
	}			
	FREE(q);
}

#if 0

struct data {
	queue_t q;
	int type; // Sender = 1, Receiver = 0
};

void *thread_func(void *data) {
	struct data *d = (struct data *) data;
	int seed = 10*d->type;
	int i = 0;
	int ret;

	printf("thread %s: started\n", d->type? "sender":"receiver");
	PacketInfo pkt;

	i = 100;
	while(i--) {
		if (d->type) {
			pkt.length = rand_r(&seed);
			printf("%d: Sending %d\n", (100-i), pkt.length);
			while (sendQueue(d->q, pkt) == -1) sleep(1);
		} else {
			while (receiveQueue(d->q, &pkt) == -1) sleep(1);
			printf("%d: Received %d\n", (100-i), pkt.length);
		}
		sleep(pkt.length%2);
	}
}


int main()
{
	int i;
	pthread_t t1, t2;
	struct data sender, receiver;

	sender.q = createQueue();

	if (sender.q == NULL) { printf("Unable to allocate queue\n"); return -1; }	

	receiver.q = sender.q;

	// Spawn sender
	sender.type = 1;
	pthread_create(&t1, NULL, thread_func, &sender);

	// Spawn Receiver
	receiver.type = 0;
	pthread_create(&t2, NULL, thread_func, &receiver);

	void *retval;
	pthread_join(t1, &retval);
	pthread_join(t2, &retval);

	destroyQueue(sender.q);
}
#endif


