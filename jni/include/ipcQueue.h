#ifndef IPC_QUEUE_H
#define IPC_QUEUE_H

struct PacketInfo{
	unsigned int length;
	void * msgPtr;
};
typedef struct PacketInfo PacketInfo;

typedef void *queue_t;

queue_t createQueue();
int sendQueue(queue_t q, PacketInfo  pkt);
int receiveQueue(queue_t q, PacketInfo * pkt);
void destroyQueue(queue_t q);

#endif  /* IPC_QUEUE_H */
