#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/linkage.h>

#define MAXSIZE 500

int queue[MAXSIZE];
int front = 0;
int rear = 0;
int i, res = 0;

SYSCALL_DEFINE1(oslab_enqueue, int, a){
	
	if(rear >= MAXSIZE){ // if queue is full
		printk(KERN_INFO "[Error] - QUEUE IS FULL------------------\n");
		return -2;
	}
	// check if the given number is already in the queue.
	for(i=front; i<rear; i++){
		if(queue[i] == a){
			printk(KERN_INFO "[Error] - ALREADY EXISTING VALUE\n");
			return a;
		}
	}
	// insert number in the queue and increase value of rear
	queue[rear] = a;
	rear++;
	printk(KERN_INFO "[System call] oslab_enqueue(); -----\n");
	printk("QUEUE FRONT ------------------\n");
	// print queue from front to rear-1
	for(i=front; i<rear; i++){
		printk("%d\n", queue[i]);
	}
	printk("QUEUE REAR ------------------\n");
	return a;
}

SYSCALL_DEFINE0(oslab_dequeue){
	if(rear == front){ // if queue is empty
		printk(KERN_INFO "[Error] - EMPTY QUEUE------------------\n");
		return -2;
	}
	// queue is not empty, return  the front value in the queue.
	res = queue[front];
	// remove the front value by overwriting the rest in order from the front.
	for(i=front+1; i<rear; i++){
		queue[i-1] = queue[i];
	}
	// reduce the rear because the queue size has been decreased.
	rear--;

	printk(KERN_INFO "[System call] oslab_dequeue(); ------\n");
	printk("QUEUE FRONT------------------\n");
	// print the queue from front to rear.
	for(i=front; i<rear; i++){
		printk("%d\n", queue[i]);
	}
	printk("QUEUE REAR------------------\n");
	return res;
}
