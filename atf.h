#ifndef __ATF_H__
#define __ATF_H__

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// ==== type definitions ====

typedef struct _linked_list
{
    int size;
    int* value;
    struct _linked_list* next;
} linked_list;

// Type for parameter constraints: expects up to 10 parameter values and returns true, if the values fulfill the constraint.
typedef bool (*constraint_t)(int, int, int, int, int, int, int, int, int, int);

// Type for a tuning parameter.
typedef struct {
  char*        name;        // name of the parameter
  int          min;         // minimum value of the parameter
  int          max;         // maximum value of the parameter
  constraint_t constraint;  // constraint for the parameter (optional)
} tp_t;

// Type for a parameter configuration.
typedef struct {
  tp_t** parameters;  // "size" pointers to tuning parameters
  int*   values;      // "size" values, one for each tuning parameter in "parameters"
  int    size;        // numer of entries in "parameters" and "values"
} configuration_t;

// Type for the search space, i.e. a collection of all valid configurations.
typedef struct {
  // Ergänzen Sie hier Ihre Lösung für Aufgabe 2
  linked_list* list; // wird benötigt in free_searchspace 
  int** combinations_array;
  int size_of_combinations;
  tp_t* parameters;
  int size;  // number of configurations in this search space
} search_space_t;
// Get configuration with "index" (index in [0..size-1]) from "search space".
configuration_t get_config(search_space_t* search_space, int index);
// Free any dynamic memory allocated for the search space.
void free_search_space(search_space_t* search_space);

// Type for cost functions: expects up to 10 parameter values and returns the cost of that configuration.
typedef int (*cost_function_t)(int, int, int, int, int, int, int, int, int, int);

// enumeration for different types of abort conditions
typedef enum {
  EVALUATIONS,  // test a set number of configurations, e.g. abort tuning after testing 50 configurations
  SECONDS,      // test configurations for a set number of seconds, e.g. abort tuning after testing configurations for 30 seconds
  MINUTES,      // test configurations for a set number of minutes
  HOURS         // test configurations for a set number of hours
} ABORT_TYPE;

// enumeration for different types of search strategies
typedef enum {
  EXHAUSTIVE,   // test configurations in order, e.g. first configuration in search space, second, third, etc.
  RANDOM        // test random configurations from search space
} SEARCH_STRATEGY;

// ==== API methods ====
tp_t            create_tp(char* name, int min, int max, constraint_t constraint);

void            generate_search_space(tp_t* parameters, int num_parameters,
                                      search_space_t* search_space);

void            explore_search_space(search_space_t* search_space, cost_function_t cost_function,
                                     SEARCH_STRATEGY search_strategy, ABORT_TYPE abort_type, int abort_value,
                                        configuration_t* best_config, int* cost);
#endif // __ATF_H__
