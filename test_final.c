#include "atf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "matmul.cpp"

// input variables
int M, N, K;
float* A;
float* B;
float* C;

// constraints for l2 cache tiling
bool ts_l2_M_divides_M(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                       int ts_l1_M, int ts_l1_N, int ts_l1_K,
                       int unused1, int unused2, int unused3, int unused4) {
  return M % ts_l2_M == 0;
}
bool ts_l2_N_divides_N(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                       int ts_l1_M, int ts_l1_N, int ts_l1_K,
                       int unused1, int unused2, int unused3, int unused4) {
  return N % ts_l2_N == 0;
}
bool ts_l2_K_divides_K(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                       int ts_l1_M, int ts_l1_N, int ts_l1_K,
                       int unused1, int unused2, int unused3, int unused4) {
  return K % ts_l2_K == 0;
}

// constraints for l1 cache tiling
bool ts_l1_M_divides_ts_l2_M(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                             int ts_l1_M, int ts_l1_N, int ts_l1_K,
                             int unused1, int unused2, int unused3, int unused4) {
  return ts_l2_M % ts_l1_M == 0;
}
bool ts_l1_N_divides_ts_l2_N(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                             int ts_l1_M, int ts_l1_N, int ts_l1_K,
                             int unused1, int unused2, int unused3, int unused4) {
  return ts_l2_N % ts_l1_N == 0;
}
bool ts_l1_K_divides_ts_l2_K(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                             int ts_l1_M, int ts_l1_N, int ts_l1_K,
                             int unused1, int unused2, int unused3, int unused4) {
  return ts_l2_K % ts_l1_K == 0;
}
int cost_function(int ts_l2_M, int ts_l2_N, int ts_l2_K,
                  int ts_l1_M, int ts_l1_N, int ts_l1_K,
                  int unused1, int unused2, int unused3, int unused4) {
  clock_t start = clock();
  tiled_matmul(M, N, K,
               A, B, C,
               ts_l2_M, ts_l2_N, ts_l2_K,
               ts_l1_M, ts_l1_N, ts_l1_K);
  int runtime = (clock() - start) * 1000.0 / CLOCKS_PER_SEC;

  return runtime;
}

int main() {
  // initialize inputs for MatMul routine
  M = N = K = 128;
  A = (float*) malloc(M * K * sizeof(float));
  B = (float*) malloc(K * N * sizeof(float));
  C = (float*) malloc(M * N * sizeof(float));

  // define tuning parameters
  tp_t parameters[] = {
      create_tp("ts_l2_M",  1, M, ts_l2_M_divides_M),
      create_tp("ts_l2_N",  1, N, ts_l2_N_divides_N),
      create_tp("ts_l2_K",  1, K, ts_l2_K_divides_K),
      create_tp("ts_l1_M",  1, M, ts_l1_M_divides_ts_l2_M),
      create_tp("ts_l1_N",  1, N, ts_l1_N_divides_ts_l2_N),
      create_tp("ts_l1_K",  1, K, ts_l1_K_divides_ts_l2_K)
  };

  // generate search space
  search_space_t search_space;
  generate_search_space(parameters, 6, &search_space);
  printf("The search space contains %d configurations.\n", search_space.size);

  // check if search space contains expected configurations
  FILE *fptr;
  fptr = fopen("final.txt", "r");
  int expected_search_space_size;
  fscanf(fptr, "%d", &expected_search_space_size);
  int* expected_configs = (int*) malloc(expected_search_space_size * 6 * sizeof(int));
  bool* found = (bool*) malloc(expected_search_space_size * sizeof(bool));
  memset(found, 0, expected_search_space_size * sizeof(bool));
  for (int i = 0; i < expected_search_space_size * 6; ++i) {
    fscanf(fptr, "%d", (&expected_configs[i]));
  }
  fclose(fptr);
  int search_space_size = search_space.size;
  if (search_space_size != expected_search_space_size) {
    printf("Die Suchraumgröße ist inkorrekt. Der Suchraum sollte %d Konfigurationen enthalten, enthält aber %d Konfigurationen.\n", expected_search_space_size, search_space_size);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < search_space_size; ++i) {
    configuration_t config = get_config(&search_space, i);
    if (config.size != 6) {
      printf("Die Anzahl der Parameter/Werte in einer Konfiguration ist falsch. Jede Konfiguration sollte 6 Parameter/Werte enthalten, Konfiguration %d enthält aber %d Parameter/Werte.\n", i, config.size);
      exit(EXIT_FAILURE);
    }
    int ts_l2_M_index = -1;
    int ts_l2_N_index = -1;
    int ts_l2_K_index = -1;
    int ts_l1_M_index = -1;
    int ts_l1_N_index = -1;
    int ts_l1_K_index = -1;
    for (int j = 0; j < 6; ++j) {
      if (strcmp(config.parameters[j]->name, "ts_l2_M") == 0)
        ts_l2_M_index = j;
      if (strcmp(config.parameters[j]->name, "ts_l2_N") == 0)
        ts_l2_N_index = j;
      if (strcmp(config.parameters[j]->name, "ts_l2_K") == 0)
        ts_l2_K_index = j;
      if (strcmp(config.parameters[j]->name, "ts_l1_M") == 0)
        ts_l1_M_index = j;
      if (strcmp(config.parameters[j]->name, "ts_l1_N") == 0)
        ts_l1_N_index = j;
      if (strcmp(config.parameters[j]->name, "ts_l1_K") == 0)
        ts_l1_K_index = j;
    }
    if (ts_l2_M_index < 0 || ts_l2_N_index < 0 || ts_l2_K_index < 0 ||
        ts_l1_M_index < 0 || ts_l1_N_index < 0 || ts_l1_K_index < 0) {
      printf("Nicht alle Parameter sind in der Konfiguration vorhanden.\n");
      if (ts_l2_M_index == -1) printf("Es fehlt Parameter ts_l2_M.\n");
      if (ts_l2_N_index == -1) printf("Es fehlt Parameter ts_l2_N.\n");
      if (ts_l2_K_index == -1) printf("Es fehlt Parameter ts_l2_K.\n");
      if (ts_l1_M_index == -1) printf("Es fehlt Parameter ts_l1_M.\n");
      if (ts_l1_N_index == -1) printf("Es fehlt Parameter ts_l1_N.\n");
      if (ts_l1_K_index == -1) printf("Es fehlt Parameter ts_l1_K.\n");
      exit(EXIT_FAILURE);
    }

    bool config_expected = false;
    for (int j = 0; j < expected_search_space_size; ++j) {
      if (expected_configs[j * 6 + 0] == config.values[ts_l2_M_index] &&
          expected_configs[j * 6 + 1] == config.values[ts_l2_N_index] &&
          expected_configs[j * 6 + 2] == config.values[ts_l2_K_index] &&
          expected_configs[j * 6 + 3] == config.values[ts_l1_M_index] &&
          expected_configs[j * 6 + 4] == config.values[ts_l1_N_index] &&
          expected_configs[j * 6 + 5] == config.values[ts_l1_K_index]) {
        if (found[j]) {
          printf("Die folgende Konfiguration ist doppelt im Suchraum vorhanden: ts_l2_M=%d, ts_l2_N=%d, ts_l2_K=%d, ts_l1_M=%d, ts_l1_N=%d, ts_l1_K=%d\n",
                 config.values[ts_l2_M_index], config.values[ts_l2_N_index], config.values[ts_l2_K_index],
                 config.values[ts_l1_M_index], config.values[ts_l1_N_index], config.values[ts_l1_K_index]);
          exit(EXIT_FAILURE);
        } else {
          found[j] = true;
          config_expected = true;
          break;
        }
      }
    }
    if (!config_expected) {
      printf("Die folgende Konfiguration befindet sich im Suchraum, sollte dort aber nicht auftauchen: ts_l2_M=%d, ts_l2_N=%d, ts_l2_K=%d, ts_l1_M=%d, ts_l1_N=%d, ts_l1_K=%d\n",
             config.values[ts_l2_M_index], config.values[ts_l2_N_index], config.values[ts_l2_K_index],
             config.values[ts_l1_M_index], config.values[ts_l1_N_index], config.values[ts_l1_K_index]);
      exit(EXIT_FAILURE);
    }
  }
  bool not_found_configs_exist = false;
  for (int i = 0; i < expected_search_space_size; ++i)
    if (!found[i]) {
      printf("Folgende Konfiguration fehlt im Suchraum: ts_l2_M=%d, ts_l2_N=%d, ts_l2_K=%d, ts_l1_M=%d, ts_l1_N=%d, ts_l1_K=%d\n",
             expected_configs[i * 6 + 0], expected_configs[i * 6 + 1], expected_configs[i * 6 + 2],
             expected_configs[i * 6 + 3], expected_configs[i * 6 + 4], expected_configs[i * 6 + 5]);
      not_found_configs_exist = true;
    }
  if (not_found_configs_exist) exit(EXIT_FAILURE);

  // start exploration
  configuration_t best_config;
  int             best_cost;
  explore_search_space(&search_space, cost_function,
                       RANDOM, EVALUATIONS, 2000,
                       &best_config, &best_cost);

  if (best_config.size != 6) {
    printf("Die beste gefundene Konfiguration sollte 6 Parameter haben, hat aber %d\n", best_config.size);
    exit(EXIT_FAILURE);
  }
  int ts_l2_M_index = -1;
  int ts_l2_N_index = -1;
  int ts_l2_K_index = -1;
  int ts_l1_M_index = -1;
  int ts_l1_N_index = -1;
  int ts_l1_K_index = -1;
  for (int j = 0; j < 6; ++j) {
    if (strcmp(best_config.parameters[j]->name, "ts_l2_M") == 0)
      ts_l2_M_index = j;
    if (strcmp(best_config.parameters[j]->name, "ts_l2_N") == 0)
      ts_l2_N_index = j;
    if (strcmp(best_config.parameters[j]->name, "ts_l2_K") == 0)
      ts_l2_K_index = j;
    if (strcmp(best_config.parameters[j]->name, "ts_l1_M") == 0)
      ts_l1_M_index = j;
    if (strcmp(best_config.parameters[j]->name, "ts_l1_N") == 0)
      ts_l1_N_index = j;
    if (strcmp(best_config.parameters[j]->name, "ts_l1_K") == 0)
      ts_l1_K_index = j;
  }
  if (ts_l2_M_index < 0 || ts_l2_N_index < 0 || ts_l2_K_index < 0 ||
      ts_l1_M_index < 0 || ts_l1_N_index < 0 || ts_l1_K_index < 0) {
    printf("Nicht alle Parameter sind in der besten Konfiguration vorhanden.\n");
    if (ts_l2_M_index == -1) printf("Es fehlt Parameter ts_l2_M.\n");
    if (ts_l2_N_index == -1) printf("Es fehlt Parameter ts_l2_N.\n");
    if (ts_l2_K_index == -1) printf("Es fehlt Parameter ts_l2_K.\n");
    if (ts_l1_M_index == -1) printf("Es fehlt Parameter ts_l1_M.\n");
    if (ts_l1_N_index == -1) printf("Es fehlt Parameter ts_l1_N.\n");
    if (ts_l1_K_index == -1) printf("Es fehlt Parameter ts_l1_K.\n");
    exit(EXIT_FAILURE);
  }

  printf("\n\n The best found configuration was:\n");
  for (int i = 0; i < best_config.size; ++i) {
    printf("  %s = %d\n", best_config.parameters[i]->name, best_config.values[i]);
  }
  printf("with cost: %d ms\n", best_cost);

  // free memory
  free_search_space(&search_space);
  free(A);
  free(B);
  free(C);

  printf("\n\nDer finale Test wurde erfolgreich ausgeführt!\n");
}
