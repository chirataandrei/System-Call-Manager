# System Call Manager

**Author:** CHIRATA ANDREI (313 CA)

## Main

I have organised my project such that in `main` I have a loop that reads the commands one by one. I used the `strstr` function to recognize the command and then call the corresponding function.

## Data Structures Functions

In `structs.h`, I wrote the definitions for every struct used: doubly linked list, stack, queue, and project-specific structures. 

In `data_struct_functions.h`, I declared the headers for the data structure operations (basic functions that add or remove elements, etc.). 

In `data_struct_functions.c`, I implemented these functions using the concepts from the lab sessions, adding support for sentinel nodes where required.

## Operations

In `operations.h`, there are the headers for the main program logic, implemented in `operations.c`:

* **`init_system`**: Dynamically allocates all structures needed for the project logic and initializes the sentinel nodes.
* **`add_incident`**: Parses input data using `strtok`, extracts the incident details, and inserts the incident into the list and its corresponding priority queue using data structure helpers.
* **`dispatch`**: Identifies the highest-priority incident, marks its status as intervened, dequeues the first available unit from the units queue, and pairs them into an active intervention.
* **`undo_last_dispatch`**: Retrieves the last dispatched incident from the history stack, re-enqueues it according to its priority, and removes the intervention node.
* **`solved_incident`**: Locates an incident by its ID and marks it as solved.
* **`show_unit` / `show_incident`**: Displays detailed information for a specific unit or incident by ID.
* **`show_interventions`**: Prints the list of all current interventions.
* **`free_all`**: Calls the cleanup functions across all allocated data structures to prevent memory leaks.