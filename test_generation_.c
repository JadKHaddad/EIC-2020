#include "atf.h"

#include <stdlib.h>
#include <stdio.h>

int test_generation()
{
  // define tuning parameters
  tp_t parameters[] = {
      create_tp("N", 16, 16, NULL),
      create_tp("TS1", 1, 16, TS1_divides_N),
      create_tp("TS2", 1, 16, TS2_less_than_or_eq_TS1)};

  // generate search space
  printf("    N  TS1  TS2\n");
  search_space_t search_space;
  generate_search_space(parameters, 3, &search_space);

  while (search_space.list->next != NULL)
  {
    printf("%d, %d, %d \n", search_space.list->value[0], search_space.list->value[1], search_space.list->value[2]);
    search_space.list = search_space.list->next;
  }

  // int* l = get_value_ll(search_space.list, 30); // no get_value_ll func defined
  // printf("%d, %d, %d \n", l[0], l[1], l[2]);

  configuration_t best_config;
  int best_cost;
  explore_search_space(&search_space, cost_function,
                       RANDOM, EVALUATIONS, search_space.size,
                       &best_config, &best_cost);

  free_search_space(&search_space);
  return 0;
}
