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

  // check if search space contains expected configurations
  int search_space_size = search_space.size;
  if (search_space_size != 31) {
    printf("Die Suchraumgröße ist inkorrekt. Der Suchraum sollte 31 Konfigurationen enthalten, enthält aber %d Konfigurationen.\n", search_space_size);
    exit(EXIT_FAILURE);
  }
  int expected_configs[][3] = {
      {16, 1, 1},
      {16, 2, 1},
      {16, 2, 2},
      {16, 4, 1},
      {16, 4, 2},
      {16, 4, 3},
      {16, 4, 4},
      {16, 8, 1},
      {16, 8, 2},
      {16, 8, 3},
      {16, 8, 4},
      {16, 8, 5},
      {16, 8, 6},
      {16, 8, 7},
      {16, 8, 8},
      {16, 16, 1},
      {16, 16, 2},
      {16, 16, 3},
      {16, 16, 4},
      {16, 16, 5},
      {16, 16, 6},
      {16, 16, 7},
      {16, 16, 8},
      {16, 16, 9},
      {16, 16, 10},
      {16, 16, 11},
      {16, 16, 12},
      {16, 16, 13},
      {16, 16, 14},
      {16, 16, 15},
      {16, 16, 16}
  };
  bool found[31];
  memset(found, 0, 31 * sizeof(bool));
  for (int i = 0; i < 31; ++i) {
    configuration_t config = get_config(&search_space, i);
    if (config.size != 3) {
      printf("Die Anzahl der Parameter/Werte in einer Konfiguration ist falsch. Jede Konfiguration sollte 3 Parameter/Werte enthalten, Konfiguration %d enthält aber %d Parameter/Werte.\n", i, config.size);
      exit(EXIT_FAILURE);
    }
    int N_index = -1;
    int TS1_index = -1;
    int TS2_index = -1;
    for (int j = 0; j < 3; ++j) {
      if (strcmp(config.parameters[j]->name, "N") == 0)
        N_index = j;
      if (strcmp(config.parameters[j]->name, "TS1") == 0)
        TS1_index = j;
      if (strcmp(config.parameters[j]->name, "TS2") == 0)
        TS2_index = j;
    }
    if (N_index < 0 || TS1_index < 0 || TS2_index < 0) {
      printf("Nicht alle Parameter sind in der Konfiguration vorhanden.\n");
      if (N_index == -1) printf("Es fehlt Parameter N.\n");
      if (TS1_index == -1) printf("Es fehlt Parameter TS1.\n");
      if (TS2_index == -1) printf("Es fehlt Parameter TS2.\n");
      exit(EXIT_FAILURE);
    }

    bool config_expected = false;
    for (int j = 0; j < 31; ++j) {
      if (expected_configs[j][0] == config.values[N_index] && expected_configs[j][1] == config.values[TS1_index] && expected_configs[j][2] == config.values[TS2_index]) {
        if (found[j]) {
          printf("Die folgende Konfiguration ist doppelt im Suchraum vorhanden: N=%d, TS1=%d, TS2=%d\n", config.values[N_index], config.values[TS1_index], config.values[TS2_index]);
          exit(EXIT_FAILURE);
        } else {
          found[j] = true;
          config_expected = true;
          break;
        }
      }
    }
    if (!config_expected) {
      printf("Die folgende Konfiguration befindet sich im Suchraum, sollte dort aber nicht auftauchen: N=%d, TS1=%d, TS2=%d\n", config.values[N_index], config.values[TS1_index], config.values[TS2_index]);
      exit(EXIT_FAILURE);
    }
  }
  bool not_found_configs_exist = false;
  for (int i = 0; i < 31; ++i)
    if (!found[i]) {
      printf("Folgende Konfiguration fehlt im Suchraum: N=%d, TS1=%d, TS2=%d\n", expected_configs[i][0], expected_configs[i][1], expected_configs[i][2]);
      not_found_configs_exist = true;
    }
  if (not_found_configs_exist) exit(EXIT_FAILURE);

  // free search space
  free_search_space(&search_space);

  printf("Die Storage-Phase wurde erfolgreich implementiert!\n");
}
