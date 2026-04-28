//CHIRATA ANDREI 313 CA
#include "operations.h"

int main(void)
{
	FILE *in = fopen("tema1.in", "rt");
	FILE *out = fopen("tema1.out", "wt");
	int num_units, num_ops;
	fscanf(in, "%d", &num_units);
	fgetc(in);
	system_t *sys = malloc(sizeof(*sys));
	DIE(!sys, "system malloc");
	unit *u = malloc(num_units * sizeof(unit));
	DIE(!u, "malloc units");
	init_system(sys);
	sys->units_array = u;
	for (int i = 0; i < num_units; i++) {
		fscanf(in, "%d %c", &u[i].id, &u[i].type);
		u[i].availability = 1;
		unit *p_u = &u[i];
		enqueue(sys->queue_available_units, p_u);
	}
	fscanf(in, "%d", &num_ops);
	fgetc(in);
	int ops_read = 0;
	while (ops_read < num_ops) {
		char input[LEN_MAX];
		fgets(input, LEN_MAX, in);
		if (strstr(input, "ADD_INCIDENT")) {
			add_incident(sys, input);
		} else if (strstr(input, "CHECK_UNITS_AVAILABILITY")) {
			check_units_availability(sys, out);
		} else if (strstr(input, "UNDO_LAST_DISPATCH")) {
			undo_last_dispatch(sys, out);
		} else if (strstr(input, "DISPATCH")) {
			dispatch(sys, out);
		} else if (strstr(input, "SOLVED_INCIDENT")) {
			solved_incident(sys, input, out);
		} else if (strstr(input, "SHOW_UNIT")) {
			show_unit(sys, input, out, num_units);
		} else if (strstr(input, "SHOW_INCIDENT")) {
			show_incident(sys, input, out);
		} else if (strstr(input, "SHOW_INTERVENTIONS")) {
			show_interventions(sys, out);
		}
		ops_read++;
	}
	free(u);
	free_all(sys);
	fclose(in);
	fclose(out);
	return 0;
}
