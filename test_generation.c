#include "atf.h"

#include <stdlib.h>
#include <stdio.h>

bool TS1_divides_N(int N, int TS1, int TS2, int unused1, int unused2, int unused3, int unused4, int unused5, int unused6, int unused7) {
  // configurations where TS1 divides N are valid
  return N % TS1 == 0;
}
bool TS2_less_than_or_eq_TS1(int N, int TS1, int TS2, int unused1, int unused2, int unused3, int unused4, int unused5, int unused6, int unused7) {
  // configurations where TS2 is less than or equal TS1 are valid
  return TS2 <= TS1;
}

int cost_function(int N, int TS1, int TS2, int unused1, int unused2, int unused3, int unused4, int unused5, int unused6, int unused7) {
    // pseudo cost function to simulate exploration
    return abs(4355 - (N * 16 * 16 + TS1 * 16 + TS2));
}

int main() {
  // define tuning parameters
  tp_t parameters[] = {
    create_tp("N",   16, 16, NULL),
    create_tp("TS1",  1, 16, TS1_divides_N),
    create_tp("TS2",  1, 16, TS2_less_than_or_eq_TS1)
  };
  
  // generate search space
  printf("    N  TS1  TS2\n");
  search_space_t search_space;
  generate_search_space(parameters, 3, &search_space);

  while (search_space.list->next != NULL) {
      printf("%d, %d, %d \n", search_space.list->value[0], search_space.list->value[1], search_space.list->value[2]);
      search_space.list = search_space.list->next;
  }

  // int* l = get_value_ll(search_space.list, 30); // no get_value_ll func defined
  // printf("%d, %d, %d \n", l[0], l[1], l[2]);

  configuration_t best_config;
  int             best_cost;
  explore_search_space(&search_space, cost_function,
      RANDOM, EVALUATIONS, search_space.size,
      &best_config, &best_cost);

  free_search_space(&search_space);
}
