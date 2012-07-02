/*
 * queue.c
 */

#include <queue.h>
SOURCE_DECL

node* LL_new(cval c, node* next) {
	node* li;
	safe_new(li, node, return NULL);
	li->data = c;
	li->next = next;
	return li;
}

void LL_free(node* li) {
	node* next;
	while (li) {
		next = li->next;
		MEM_free(li);
		li = next;
	}
}

void LL_push_front(node** li, cval data) {
	node* tmp = *li;
	*li = LL_new(data, tmp);
}

void LL_pop_front(node** li) {
	node* tmp = *li;
	*li = (*li)->next;
	MEM_free(tmp);
}

void LL_foreach(node* li, foreach_fn fn, cval ctx) {
	for (node* cur = li; cur != NULL; cur = cur->next) {
		fn(cur->data, ctx);
	}
}

void LL_print(node* li) {
	printf("(");	
	for (node* cur = li; cur != NULL; cur = cur->next) {
		printf("%p", cur->data.ptr);
		if (cur->next) {
			printf(", ");
		}
	}
	printf(")");
}

queue* Q_new() {
	return MEM_calloc(1, sizeof(queue));
}

void Q_free(queue* q) {
	while (q->head) {
		Q_pop_front(q);
	}
}

cval Q_front(queue* q) {
	return (q->head == NULL) ? nil : q->head->data;
}

cval Q_back(queue* q) {
	return (q->tail == NULL) ? nil : q->tail->data;
}

static void dir_push(queue* q, cval data, bool to_front) {
	node* p = LL_new(data, NULL);
	if (NULL == p) return;

	if (NULL == q->head && NULL == q->tail) {
		q->head = q->tail = p;
	} else if (NULL == q->head || NULL == q->tail) {
		return;
	} else {
		if (to_front) {
			p->next = q->head;
			q->head = p;
		} else {
			q->tail->next = p;
			q->tail = p;
		}
	}
	
	++q->size;
}	

void Q_push_front(queue* q, cval data) {
	dir_push(q, data, true);
}

void Q_push_back(queue* q, cval data) {
	dir_push(q, data, false);
}

void Q_pop_front(queue* q) {
	node* h = q->head;
	node* p = h->next;

	MEM_free(h);
	q->head = p;
	
	if (NULL == q->head) {
		q->tail = NULL;
	}
	
	--q->size;
}

void Q_remove(queue* q, foreach_fn fn, cval ctx) {
	for (node* p = q->head; p != NULL; p = p->next) {
		if (fn(p->data, ctx)) {
			node* tmp = p;
			p = p->next;
			MEM_free(tmp);
			--q->size;
		}
	}
}

void Q_foreach(queue* q, foreach_fn fn, cval ctx) {
	for (node* p = q->head; p != NULL; p = p->next) {
		fn(p->data, ctx);
	}
}

void Q_print(queue* q) {
	LL_print(q->head);
}

SOURCE_STOP
