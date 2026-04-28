//CHIRATA ANDREI 313 CA
#include "data_struct_functions.h"

//function that creates a doubly linked list with sentinel node
dll_list_t *dll_create(void *sentinel_data)
{
	dll_list_t *new_list = malloc(sizeof(*new_list));
	DIE(!new_list, "dll_create malloc");
	new_list->sentinel = malloc(sizeof(*new_list->sentinel));
	DIE(!new_list->sentinel, "dll_create malloc");
	new_list->size = 0;
	new_list->sentinel->next = new_list->sentinel;
	new_list->sentinel->prev = new_list->sentinel;
	new_list->sentinel->data = sentinel_data;
	return new_list;
}

//function that adds a node in a doubly linked list
void dll_add_node(dll_list_t *list, void *new_data)
{
	if (!list || !list->sentinel)
		return;
	dll_node_t *new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "ddl_add_node malloc");
	new_node->data = new_data;
	dll_node_t *last_node = list->sentinel->prev;
	new_node->prev = last_node;
	new_node->next = list->sentinel;
	last_node->next = new_node;
	list->sentinel->prev = new_node;
	list->size++;
}

//function that removes a specific node from a doubly linked list
void dll_remove_specific_node(dll_list_t *list, void *target_data)
{
	if (!list || list->size == 0)
		return;
	dll_node_t *curr_node = list->sentinel->next;
	while (curr_node != list->sentinel) {
		if (curr_node->data == target_data) {
			dll_node_t *prev = curr_node->prev;
			dll_node_t *next = curr_node->next;
			prev->next = next;
			next->prev = prev;
			free(curr_node);
			list->size--;
			return;
		}
		curr_node = curr_node->next;
	}
}

//function that returns the size of a doubly linked list
unsigned int dll_get_size(dll_list_t *list)
{
	return list->size;
}

//function that frees a list
void dll_free(dll_list_t **pp_list)
{
	if (!pp_list || !*pp_list)
		return;
	dll_node_t *curr = (*pp_list)->sentinel->next;
	while (curr != (*pp_list)->sentinel) {
		dll_node_t *to_remove = curr;
		curr = curr->next;
		if (to_remove->data)
			free(to_remove->data);
		free(to_remove);
	}
	if ((*pp_list)->sentinel->data)
		free((*pp_list)->sentinel->data);
	free((*pp_list)->sentinel);
	free(*pp_list);
	*pp_list = NULL;
}

//functions that creates a queue
my_queue_t *init_queue(void)
{
	my_queue_t *new_queue = malloc(sizeof(*new_queue));
	DIE(!new_queue, "init_queue malloc");
	new_queue->front = NULL;
	new_queue->rear = NULL;
	new_queue->size = 0;
	return new_queue;
}

//function that adds an element to the queue
void enqueue(my_queue_t *q, void *new_data)
{
	if (!q)
		return;
	queue_node_t *new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "enqueue malloc");
	new_node->data = new_data;
	new_node->next = NULL;
	if (!q->size) {
		q->front = new_node;
		q->rear = new_node;
		q->size++;
		return;
	}
	q->rear->next = new_node;
	q->rear = new_node;
	q->size++;
}

//function that adds an element in the front of the queue
void enqueue_front(my_queue_t *q, void *new_data)
{
	if (!q)
		return;
	queue_node_t *new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "enqueue_front malloc");
	new_node->data = new_data;
	new_node->next = q->front;
	q->front = new_node;
	if (q->size == 0)
		q->rear = new_node;
	q->size++;
}

//function that removes an element from the queue
void *dequeue(my_queue_t *q)
{
	if (!q || q->size == 0)
		return NULL;
	queue_node_t *to_remove = q->front;
	void *extract_data = to_remove->data;
	q->front = q->front->next;
	free(to_remove);
	q->size--;
	if (!q->front)
		q->rear = NULL;
	return extract_data;
}

//function that returns the size of the queue
unsigned int queue_get_size(my_queue_t *q)
{
	return q->size;
}

//function that frees a queue
void q_free(my_queue_t *q)
{
	if (!q)
		return;
	queue_node_t *curr = q->front;
	while (curr) {
		queue_node_t *to_remove = curr;
		curr = curr->next;
		free(to_remove);
	}
	free(q);
}

//function that creates a stack
my_stack_t *init_stack(void)
{
	my_stack_t *new_stack = malloc(sizeof(*new_stack));
	DIE(!new_stack, "init_stack malloc");
	new_stack->size = 0;
	new_stack->top = NULL;
	return new_stack;
}

//function that adds an element to the stack
void st_push(my_stack_t *st, void *new_data)
{
	if (!st)
		return;
	stack_node_t *new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "push malloc");
	new_node->data = new_data;
	new_node->next = st->top;
	st->top = new_node;
	st->size++;
}

//function that removes an element from the stack
void *st_pop(my_stack_t *st)
{
	if (!st || st->size == 0)
		return NULL;
	stack_node_t *to_remove = st->top;
	void *extract_data = to_remove->data;
	st->top = st->top->next;
	free(to_remove);
	st->size--;
	return extract_data;
}

//function that frees a stack
void st_free(my_stack_t *st)
{
	if (!st)
		return;
	stack_node_t *curr = st->top;
	while (curr) {
		stack_node_t *to_remove = curr;
		curr = curr->next;
		free(to_remove);
	}
	free(st);
}
