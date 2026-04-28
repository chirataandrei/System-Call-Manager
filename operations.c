//CHIRATA ANDREI 313 CA
#include "operations.h"

//function that creates the system
void init_system(system_t *sys)
{
	if (!sys)
		return;
	sys->units_array = NULL;
	//alloc the sentinel node for incidents' list
	incident_t *sentinel_inc = malloc(sizeof(*sentinel_inc));
	DIE(!sentinel_inc, "init_system malloc");
	sentinel_inc->id = 0;
	strcpy(sentinel_inc->priority, "low");
	sentinel_inc->description = "test incident";
	strcpy(sentinel_inc->status, "solved");
	sys->incidents_list = dll_create(sentinel_inc);
	//alloc the sentinel for interventions' list
	intervention_t *sentinel_int = malloc(sizeof(*sentinel_int));
	DIE(!sentinel_int, "init_system malloc");
	sentinel_int->target_incident = NULL;
	sentinel_int->assigned_team = NULL;
	sys->interventions_list = dll_create(sentinel_int);
	//alloc the queues
	sys->queue_low = init_queue();
	sys->queue_medium = init_queue();
	sys->queue_high = init_queue();
	sys->queue_available_units = init_queue();
	//alloc the stack
	sys->stack_history = init_stack();
}

//function that adds an incident to the incident's list and to the queue
void add_incident(system_t *sys, char *input)
{
	incident_t *new_incident = malloc(sizeof(*new_incident));
	DIE(!new_incident, "add_incident malloc");
	//extract the new incident
	char *second = strtok(input, " ");
	second = strtok(NULL, " ");
	char *third = strtok(NULL, " ");
	char *description = strtok(NULL, "\n");
	new_incident->id = atoi(second);
	strcpy(new_incident->priority, third);
	strcpy(new_incident->status, "queued");
	new_incident->description = malloc(strlen(description) + 1);
	DIE(!new_incident->description, "malloc descriere");
	strcpy(new_incident->description, description);
	//add the new incident to the list
	dll_add_node(sys->incidents_list, new_incident);
	//add the new incident to the queue
	char *priority = third;
	if (strcmp(priority, "low") == 0)
		enqueue(sys->queue_low, new_incident);
	else if (strcmp(priority, "medium") == 0)
		enqueue(sys->queue_medium, new_incident);
	else if (strcmp(priority, "high") == 0)
		enqueue(sys->queue_high, new_incident);
}

//function that prints the number of available units
void check_units_availability(system_t *sys, FILE *out)
{
	fprintf(out, "Number of available units: %d\n",
			sys->queue_available_units->size);
}

//function that dispatches a unit to an incident
void dispatch(system_t *sys, FILE *out)
{
	if (queue_get_size(sys->queue_available_units) == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	if (queue_get_size(sys->queue_high) == 0 &&
	    queue_get_size(sys->queue_medium) == 0 &&
	    queue_get_size(sys->queue_low) == 0) {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
		return;
	}
	//find the incident with biggest priority
	incident_t *curr_inc = NULL;
	if (queue_get_size(sys->queue_high))
		curr_inc = dequeue(sys->queue_high);
	else if (queue_get_size(sys->queue_medium))
		curr_inc = dequeue(sys->queue_medium);
	else
		curr_inc = dequeue(sys->queue_low);
	unit *curr_u = dequeue(sys->queue_available_units);
	//update incident and unit status
	strcpy(curr_inc->status, "intervened");
	curr_u->availability = 0;
	//create a new intervention
	intervention_t *new_int = malloc(sizeof(*new_int));
	new_int->assigned_team = curr_u;
	new_int->target_incident = curr_inc;
	dll_add_node(sys->interventions_list, new_int);
	//add to stack history
	st_push(sys->stack_history, new_int);
}

//function that undoes the last active dispatch
void undo_last_dispatch(system_t *sys, FILE *out)
{
	//find the intervion that will be canceled
	intervention_t *int_to_cancel = NULL;
	while (sys->stack_history->size > 0) {
		intervention_t *curr_int = (intervention_t *)st_pop(sys->stack_history);
		if (strcmp(curr_int->target_incident->status, "intervened") == 0) {
			int_to_cancel = curr_int;
			break;
		}
	}
	//change its status and adds it to the specific queue
	if (int_to_cancel) {
		strcpy(int_to_cancel->target_incident->status, "queued");
		if (!strcmp(int_to_cancel->target_incident->priority, "high"))
			enqueue_front(sys->queue_high, int_to_cancel->target_incident);
		else if (!strcmp(int_to_cancel->target_incident->priority, "medium"))
			enqueue_front(sys->queue_medium, int_to_cancel->target_incident);
		else if (!strcmp(int_to_cancel->target_incident->priority, "low"))
			enqueue_front(sys->queue_low, int_to_cancel->target_incident);
		int_to_cancel->assigned_team->availability = 1;
		enqueue(sys->queue_available_units, int_to_cancel->assigned_team);
		dll_remove_specific_node(sys->interventions_list, int_to_cancel);
		free(int_to_cancel);
	} else {
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
	}
}

//function that finds an incident with a specific id and marks it
void solved_incident(system_t *sys, char *input, FILE *out)
{
	char *second = strtok(input, " ");
	second = strtok(NULL, "\n");
	int target_id = atoi(second);
	dll_node_t *curr_node = sys->interventions_list->sentinel->next;
	while (curr_node != sys->interventions_list->sentinel) {
		intervention_t *curr_int = (intervention_t *)curr_node->data;
		if (curr_int->target_incident->id == target_id &&
		    strcmp(curr_int->target_incident->status, "intervened") == 0) {
			strcpy(curr_int->target_incident->status, "solved");
			curr_int->assigned_team->availability = 1;
			enqueue(sys->queue_available_units, curr_int->assigned_team);
			break;
		}
		curr_node = curr_node->next;
	}
	if (curr_node == sys->interventions_list->sentinel)
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

//function that prints an unit with a specific id
void show_unit(system_t *sys, char *input, FILE *out, int num_units)
{
	char *second = strtok(input, " ");
	second = strtok(NULL, "\n");
	int target_id = atoi(second);
	unit *curr_unit = sys->units_array;
	unit *end = sys->units_array + num_units;
	while (curr_unit < end) {
		if (curr_unit->id == target_id) {
			char *av = (curr_unit->availability == 1) ?
				   "available" : "unavailable";
			fprintf(out, "Unit %d is type %c and is %s\n",
					curr_unit->id, curr_unit->type, av);
			break;
		}
		curr_unit++;
	}
	if (curr_unit == end)
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

//function that prints an incident with a specific id
void show_incident(system_t *sys, char *input, FILE *out)
{
	char *second = strtok(input, " ");
	second = strtok(NULL, "\n");
	int target_id = atoi(second);
	dll_node_t *curr_node = sys->incidents_list->sentinel->next;
	while (curr_node != sys->incidents_list->sentinel) {
		incident_t *curr_inc = (incident_t *)curr_node->data;
		if (curr_inc->id == target_id) {
			fprintf(out, "Incident %d has %s priority, the "
				"following description: %s and is %s\n",
				curr_inc->id, curr_inc->priority,
				curr_inc->description, curr_inc->status);
			break;
		}
		curr_node = curr_node->next;
	}
	if (curr_node == sys->incidents_list->sentinel)
		fprintf(out, "INVALID OPERATION! ERROR 404\n");
}

//function that prints all the interventions
void show_interventions(system_t *sys, FILE *out)
{
	dll_node_t *curr_node = sys->interventions_list->sentinel->next;
	if (curr_node == sys->interventions_list->sentinel) {
		fprintf(out, "No intervention has been initiated\n");
		return;
	}
	while (curr_node != sys->interventions_list->sentinel) {
		intervention_t *curr_int = (intervention_t *)curr_node->data;
		fprintf(out, "Incident %d was assigned to unit %d, and "
			"has the following status: \"%s\"\n",
			curr_int->target_incident->id,
			curr_int->assigned_team->id,
			curr_int->target_incident->status);
		curr_node = curr_node->next;
	}
}

//function that frees the incidents' list
void free_incidents(dll_list_t **pp_list)
{
	if (!pp_list || !*pp_list)
		return;
	dll_node_t *curr_node = (*pp_list)->sentinel->next;
	while (curr_node != (*pp_list)->sentinel) {
		dll_node_t *to_remove = curr_node;
		curr_node = curr_node->next;
		if (to_remove->data) {
			incident_t *inc = (incident_t *)to_remove->data;
			if (inc->description)
				free(inc->description);
			free(inc);
		}
		free(to_remove);
	}
	if ((*pp_list)->sentinel->data)
		free((*pp_list)->sentinel->data);
	free((*pp_list)->sentinel);
	free(*pp_list);
	*pp_list = NULL;
}

//function that frees all the elements
void free_all(system_t *sys)
{
	if (!sys)
		return;
	q_free(sys->queue_high);
	q_free(sys->queue_medium);
	q_free(sys->queue_low);
	q_free(sys->queue_available_units);
	st_free(sys->stack_history);
	free_incidents(&sys->incidents_list);
	dll_free(&sys->interventions_list);
	free(sys);
}
