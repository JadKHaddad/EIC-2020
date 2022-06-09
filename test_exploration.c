#include "atf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool TS1_divides_N(int N, int TS1, int TS2, int unused1, int unused2, int unused3, int unused4, int unused5, int unused6, int unused7) {
  return N % TS1 == 0;
}
bool TS2_less_than_or_eq_TS1(int N, int TS1, int TS2, int unused1, int unused2, int unused3, int unused4, int unused5, int unused6, int unused7) {
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
  search_space_t search_space;
  generate_search_space(parameters, 3, &search_space);

  // start exploration
  configuration_t best_config;
  int             best_cost;
  explore_search_space(&search_space, cost_function,
                       EXHAUSTIVE, EVALUATIONS, search_space.size,
                       &best_config, &best_cost);

  if (best_config.size != 3) {
    printf("Die beste gefundene Konfiguration sollte 3 Parameter haben, hat aber %d\n", best_config.size);
    exit(EXIT_FAILURE);
  }
  int N_index = -1;
  int TS1_index = -1;
  int TS2_index = -1;
  for (int j = 0; j < 3; ++j) {
    if (strcmp(best_config.parameters[j]->name, "N") == 0)
      N_index = j;
    if (strcmp(best_config.parameters[j]->name, "TS1") == 0)
      TS1_index = j;
    if (strcmp(best_config.parameters[j]->name, "TS2") == 0)
      TS2_index = j;
  }
  if (N_index < 0 || TS1_index < 0 || TS2_index < 0) {
    printf("Nicht alle Parameter sind in der besten Konfiguration vorhanden.\n");
    if (N_index == -1) printf("Es fehlt Parameter N.\n");
    if (TS1_index == -1) printf("Es fehlt Parameter TS1.\n");
    if (TS2_index == -1) printf("Es fehlt Parameter TS2.\n");
    exit(EXIT_FAILURE);
  }
  if (best_config.values[N_index] != 16 || best_config.values[TS1_index] != 16 || best_config.values[TS2_index] != 3) {
    printf("Es wurde nicht die richtige beste Konfiguration gefunden.\n");
    printf("Es wurde folgende beste Konfiguration erwartet: N=%d, TS1=%d, TS2=%d.\n", 16, 16, 3);
    printf("Es wurde folgende beste Konfiguration gefunden: N=%d, TS1=%d, TS2=%d.\n", best_config.values[N_index], best_config.values[TS1_index], best_config.values[TS2_index]);
    exit(EXIT_FAILURE);
  }

  // free search space
  free_search_space(&search_space);

  printf("Die Exploration-Phase wurde erfolgreich implementiert!\n");
}
