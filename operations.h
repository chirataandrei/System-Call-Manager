//CHIRATA ANDREI 313 CA
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "data_struct_functions.h"

void init_system(system_t *sys);
void add_incident(system_t *sys, char *input);
void check_units_availability(system_t *sys, FILE *out);
void dispatch(system_t *sys, FILE *out);
void undo_last_dispatch(system_t *sys, FILE *out);
void solved_incident(system_t *sys, char *input, FILE *out);
void show_unit(system_t *sys, char *input, FILE *out, int num_units);
void show_incident(system_t *sys, char *input, FILE *out);
void show_interventions(system_t *sys, FILE *out);
void free_incidents(dll_list_t **pp_list);
void free_all(system_t *sys);
#endif
