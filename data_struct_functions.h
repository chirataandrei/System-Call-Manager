//CHIRATA ANDREI 313 CA

#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include "structs.h"

//Doubly linked list functions
dll_list_t *dll_create(void *sentinel_data);
void dll_add_node(dll_list_t *list, void *new_data);
void dll_remove_specific_node(dll_list_t *list, void *target_data);
unsigned int dll_get_size(dll_list_t *list);
void dll_free(dll_list_t **pp_list);

//Queue functions
my_queue_t *init_queue(void);
void enqueue(my_queue_t *q, void *new_data);
void enqueue_front(my_queue_t *q, void *new_data);
void *dequeue(my_queue_t *q);
unsigned int queue_get_size(my_queue_t *q);
void q_free(my_queue_t *q);

//Stack functions
my_stack_t *init_stack(void);
void st_push(my_stack_t *st, void *new_data);
void *st_pop(my_stack_t *st);
void st_free(my_stack_t *st);
#endif
