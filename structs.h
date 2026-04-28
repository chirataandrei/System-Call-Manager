//CHIRATA ANDREI 313 CA
#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#define LEN_MAX 2048
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

//struct that stores a unit
typedef struct {
	int id;
	char type;
	int availability;
} unit;

//doubly linked list node
typedef struct dll_node_t {
	void *data;
	struct dll_node_t *prev, *next;
} dll_node_t;

//doubly linked list
typedef struct {
	dll_node_t *sentinel;
	unsigned int size;
} dll_list_t;

//struct that stores an incident
typedef struct {
	int id;
	char priority[7];
	char *description;
	char status[11];
} incident_t;

//struct that stores an intervention
typedef struct {
	incident_t *target_incident;
	unit *assigned_team;
} intervention_t;

//struct for nodes in a queue
typedef struct queue_node {
	void *data;
	struct queue_node *next;
} queue_node_t;

//struct that stores a queue
typedef struct {
	queue_node_t *front;
	queue_node_t *rear;
	unsigned int size;
} my_queue_t;

//struct for nodes in a stack
typedef struct stack_node {
	void *data;
	struct stack_node *next;
} stack_node_t;

//struct that stores a stack
typedef struct {
	stack_node_t *top;
	unsigned int size;
} my_stack_t;

//struct that stores the sistem
typedef struct {
	unit *units_array;
	dll_list_t *incidents_list;
	dll_list_t *interventions_list;
	my_queue_t *queue_high;
	my_queue_t *queue_medium;
	my_queue_t *queue_low;
	my_queue_t *queue_available_units;
	my_stack_t *stack_history;
} system_t;
#endif
