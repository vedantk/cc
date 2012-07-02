/*
 * queue.h
 *
 * An assortment of linked-list structures.
 */ 

#ifndef QUEUE_H
#define QUEUE_H

#include <basic.h>
SOURCE_DECL

typedef struct node node;

struct node {
	cval	data;
	node*	next;
};

typedef struct {
	u32	size;
	node*	head;
	node*	tail;
} queue;

node*	LL_new(cval c, node* next);
void	LL_free(node* li);
void	LL_push_front(node** li, cval data);
void	LL_pop_front(node** li);
void	LL_foreach(node* li, foreach_fn fn, cval ctx);
void	LL_print(node* li);

queue*	Q_new();
void	Q_free(queue* q);
cval	Q_front(queue* q);
cval	Q_back(queue* q);
void	Q_push_front(queue* q, cval data);
void	Q_push_back(queue* q, cval data);
void	Q_pop_front(queue* q);
void	Q_remove(queue* q, foreach_fn fn, cval ctx);
void	Q_foreach(queue* q, foreach_fn fn, cval ctx);
void	Q_print(queue* q);

SOURCE_STOP

#endif /* QUEUE_H */
