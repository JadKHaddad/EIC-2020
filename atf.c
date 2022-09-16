#include "atf.h"

#include <stdio.h>
#include <time.h>

//------------------------------------------------------------------------------------------
void check(void* ptr, char* function_name) {

	if (ptr == NULL)
	{
		printf("\n\nError: passed invalid linked list to function %s\n\n", function_name);
		exit(1);
	}
}

//------------------------------------------------------------------------------------------
linked_list* generate_ll(int* value) {

	linked_list* ll = (linked_list*)malloc(sizeof(*ll));

	if (ll == NULL)
	{
		printf("error in malloc\n");
		exit(1);
	}

	ll->value = value;
	ll->next = NULL;

	return ll;
}
//------------------------------------------------------------------------------------------

void delete_ll(linked_list* ll) {

	check(ll, "delete_ll");

	while (ll->next)
	{
		linked_list* current = ll;
		ll = ll->next;
		free(current->value);
		free(current);
	}
}
//------------------------------------------------------------------------------------------

void print_configuration(int* values, int num_values) {

	for (int i = 0; i < num_values; ++i) {
		printf("%5d", values[i]);
	}
	printf("\n");
}
//------------------------------------------------------------------------------------------

tp_t create_tp(char* name, int min, int max, constraint_t constraint) {

	tp_t created;
	created.name = name;
	created.max = max;
	created.min = min;
	created.constraint = constraint;
	return created;
}
//------------------------------------------------------------------------------------------

void put(linked_list* serch_space_values, int** values_lists, int num_parameters,
	int i0, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8, int i9) {

	int* result = (int*)malloc(num_parameters * sizeof(int));
	result[0] = values_lists[0][i0];
	result[1] = values_lists[1][i1];
	if (num_parameters > 2) {
		result[2] = values_lists[2][i2];
		if (num_parameters > 3) {
			result[3] = values_lists[3][i3];
			if (num_parameters > 4) {
				result[4] = values_lists[4][i4];
				if (num_parameters > 5) {
					result[5] = values_lists[5][i5];
					if (num_parameters > 6) {
						result[6] = values_lists[6][i6];
						if (num_parameters > 7) {
							result[7] = values_lists[7][i7];
							if (num_parameters > 8) {
								result[8] = values_lists[8][i8];
								if (num_parameters > 9) {
									result[9] = values_lists[9][i9];
								}
							}
						}
					}
				}
			}
		}
	}
	
	serch_space_values->value = result;
	//print_configuration(result, num_parameters);
}
//------------------------------------------------------------------------------------------

void generate_search_space(tp_t* parameters, int num_parameters,
	search_space_t * search_space) {
		{ //SLOW, USE FAST
			/* 
			search_space->parameters = parameters;
			search_space->size_of_combinations = num_parameters;
			linked_list* serch_space_values = generate_ll(NULL);
			linked_list* current_ll = serch_space_values;
			serch_space_values->size = num_parameters;
			search_space->list = serch_space_values;
			int size = 1;
			{
				int a[10] = { 0 };
				for (int i = 0; i < num_parameters; i++) {
					a[i] = parameters[i].min;
				}
				//print_configuration(&a, num_parameters);
				int* result = (int*)malloc(num_parameters * sizeof(int));
				for (int i = 0; i < num_parameters; ++i) {
					result[i] = a[i];
				}
				serch_space_values->value = result;
				serch_space_values->next = generate_ll(NULL);
				serch_space_values = serch_space_values->next;
				int l = 0;
				while (true)
				{
					if (a[l] == parameters[l].max) {
						while (l < num_parameters && a[l] == parameters[l].max) {
							a[l] = parameters[l].min;
							l++;
						}
						if (l == num_parameters) break;
						a[l]++;
						l = 0;
					}
					else {
						a[l]++;
					}
					bool constraints_satisfied = true;
					// Constraints testen
					for (int i = 0; i < num_parameters; i++) {
						if (parameters[i].constraint != NULL) {
							bool constraint_i_satisfied = parameters[i].constraint(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
							if (!constraint_i_satisfied) {
								constraints_satisfied = false;
							}
						}
					}
					if (constraints_satisfied) {
						size++;
						//print_configuration(&a, num_parameters);
						int* result = (int*)malloc(num_parameters * sizeof(int));
						for (int i = 0; i < num_parameters; ++i) {
							result[i] = a[i];
						}
						serch_space_values->value = result;
						serch_space_values->next = generate_ll(NULL);
						serch_space_values = serch_space_values->next;
					}
				}
			}
			search_space->size = size;
			// ein zugriff auf array ist schneller als ein linked_list (get_configuration)
			int** combinations = (int**)malloc(size * sizeof(int*));
			int index = 0;
			while (current_ll->next) {
				combinations[index] = current_ll->value;
				++index;
				current_ll = current_ll->next;
			}

			search_space->combinations_array = combinations;
			*/
		}
		{ // FAST
			
			if (num_parameters <= 1 || num_parameters > 10) {
				return;
			}

			int serch_space_size = 0;
			
			linked_list* serch_space_values = generate_ll(NULL);
			linked_list* current_ll = serch_space_values;

			search_space->list = serch_space_values;
			
			search_space->parameters = parameters;
			search_space->size_of_combinations = num_parameters;

			int min = 2147483647; //INT_MAX
			int max = -2147483648; //INT_MIN
			for (int i = 0; i < num_parameters; ++i) {
				min = MIN(min, parameters[i].min);
				max = MAX(max, parameters[i].max);
			}

			int search_space_size_of_values = 0;
			int* size_of_values = (int*)malloc(num_parameters * sizeof(int));

			for (int i = 0; i < num_parameters; ++i) {
				size_of_values[i] = parameters[i].max - parameters[i].min + 1;
				search_space_size_of_values = search_space_size_of_values + parameters[i].max - parameters[i].min + 1;
			}

			int* values;
			int size = 0;

			for (int i = 0; i < num_parameters; ++i) {
				size = size + size_of_values[i];
			}

			values = (int*)malloc(size * sizeof(int));

			for (int i = 0; i < num_parameters; ++i) {
				int index = 0;
				for (int j = min; j <= max; ++j) {
					if (j >= parameters[i].min && j <= parameters[i].max) {
						int pre_size = 0;
						for (int k = 0; k < i; k++) {
							pre_size = pre_size + size_of_values[k];
						}
						values[pre_size + index] = j;
						++index;
					}
				}
			}

			// Passende Werte für jedes Parameter
			int** values_lists = (int**)malloc(num_parameters * sizeof(int*));

			for (int i = 0; i < num_parameters; ++i) {
				values_lists[i] = (int*)malloc(size_of_values[i] * sizeof(int));
				int index = 0;
				for (int j = 0; j < size_of_values[i]; ++j) {
					int pre_size = 0;
					for (int k = 0; k < i; k++) {
						pre_size = pre_size + size_of_values[k];
					}
					values_lists[i][j] = values[pre_size + index];
					index++;
				}
			}

			// Überprüfung
			for (int i0 = 0; i0 < size_of_values[0]; ++i0) {
				if (num_parameters > 1) {
					for (int i1 = 0; i1 < size_of_values[1]; ++i1) {
						if (num_parameters == 2) {
							bool constraints_satisfied = true;
							for (int i = 0; i < num_parameters; i++) {
								if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], 0, 0, 0, 0, 0, 0, 0, 0) == false) {
									constraints_satisfied = false;
									break;
								}
							}
							if (constraints_satisfied == true) {
								put(serch_space_values, values_lists, num_parameters, i0, i1, 0, 0, 0, 0, 0, 0, 0, 0);
								serch_space_values->next = generate_ll(NULL);
								serch_space_values = serch_space_values->next;
								++serch_space_size;
							}
						}
						else {
								if (parameters[1].constraint(values_lists[0][i0], values_lists[1][i1], 0, 0, 0, 0, 0, 0, 0, 0) == true) {
									if (num_parameters > 2) {
										for (int i2 = 0; i2 < size_of_values[2]; ++i2) {
											if (num_parameters == 3) {
												bool constraints_satisfied = true;
												for (int i = 0; i < num_parameters; i++) {
													if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], 0, 0, 0, 0, 0, 0, 0) == false) {
														constraints_satisfied = false;
														break;
													}
												}
												if (constraints_satisfied == true) {
													put(serch_space_values, values_lists, num_parameters, i0, i1, i2, 0, 0, 0, 0, 0, 0, 0);
													serch_space_values->next = generate_ll(NULL);
													serch_space_values = serch_space_values->next;
													++serch_space_size;
												}
											}
											else {
													if (parameters[2].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], 0, 0, 0, 0, 0, 0, 0) == true) {
														if (num_parameters > 3) {
															for (int i3 = 0; i3 < size_of_values[3]; ++i3) {
																if (num_parameters == 4) {
																	bool constraints_satisfied = true;
																	for (int i = 0; i < num_parameters; i++) {
																		if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], 0, 0, 0, 0, 0, 0) == false) {
																			constraints_satisfied = false;
																			break;
																		}
																	}
																	if (constraints_satisfied == true) {
																		put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, 0, 0, 0, 0, 0, 0);
																		serch_space_values->next = generate_ll(NULL);
																		serch_space_values = serch_space_values->next;
																		++serch_space_size;
																	}
																}
																else {
																		if (parameters[3].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], 0, 0, 0, 0, 0, 0) == true) {
																			if (num_parameters > 4) {
																				for (int i4 = 0; i4 < size_of_values[4]; ++i4) {
																					if (num_parameters == 5) {
																						bool constraints_satisfied = true;
																						for (int i = 0; i < num_parameters; i++) {
																							if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], 0, 0, 0, 0, 0) == false) {
																								constraints_satisfied = false;
																								break;
																							}
																						}
																						if (constraints_satisfied == true) {
																							put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, 0, 0, 0, 0, 0);
																							serch_space_values->next = generate_ll(NULL);
																							serch_space_values = serch_space_values->next;
																							++serch_space_size;
																						}
																					}
																					else {
																							if (parameters[4].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], 0, 0, 0, 0, 0) == true) {
																								if (num_parameters > 5) {
																									for (int i5 = 0; i5 < size_of_values[5]; ++i5) {
																										if (num_parameters == 6) {
																											bool constraints_satisfied = true;
																											for (int i = 0; i < num_parameters; i++) {
																												if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], 0, 0, 0, 0) == false) {
																													constraints_satisfied = false;
																													break;
																												}
																											}
																											if (constraints_satisfied == true) {
																												put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, i5, 0, 0, 0,0);
																												serch_space_values->next = generate_ll(NULL);
																												serch_space_values = serch_space_values->next;
																												++serch_space_size;
																											}
																										}
																										else {
																												if (parameters[5].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], 0, 0, 0, 0) == true) {
																													if (num_parameters > 6) {
																														for (int i6 = 0; i6 < size_of_values[6]; ++i6) {
																															if (num_parameters == 7) {
																																bool constraints_satisfied = true;
																																for (int i = 0; i < num_parameters; i++) {
																																	if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], 0, 0, 0) == false) {
																																		constraints_satisfied = false;
																																		break;
																																	}
																																}
																																if (constraints_satisfied == true) {
																																	put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, i5, i6, 0, 0, 0);
																																	serch_space_values->next = generate_ll(NULL);
																																	serch_space_values = serch_space_values->next;
																																	++serch_space_size;
																																}
																															}
																															else {
																																	if (parameters[6].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], 0, 0, 0) == true) {
																																		if (num_parameters > 7) {
																																			for (int i7 = 0; i7 < size_of_values[7]; ++i7) {
																																				if (num_parameters == 8) {
																																					bool constraints_satisfied = true;
																																					for (int i = 0; i < num_parameters; i++) {
																																						if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], values_lists[7][i7], 0, 0) == false) {
																																							constraints_satisfied = false;
																																							break;
																																						}
																																					}
																																					if (constraints_satisfied == true) {
																																						put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, i5, i6, i7, 0, 0);
																																						serch_space_values->next = generate_ll(NULL);
																																						serch_space_values = serch_space_values->next;
																																						++serch_space_size;
																																					}
																																				}
																																				else {
																																					if (parameters[7].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], values_lists[7][i7], 0, 0) == true) {
																																						if (num_parameters > 8) {
																																							for (int i8 = 0; i8 < size_of_values[8]; ++i8) {
																																								if (num_parameters == 9) {
																																									bool constraints_satisfied = true;
																																									for (int i = 0; i < num_parameters; i++) {
																																										if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], values_lists[7][i7], values_lists[8][i8], 0) == false) {
																																											constraints_satisfied = false;
																																											break;
																																										}
																																									}
																																									if (constraints_satisfied == true) {
																																										put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, i5, i6, i7, i8, 0);
																																										serch_space_values->next = generate_ll(NULL);
																																										serch_space_values = serch_space_values->next;
																																										++serch_space_size;
																																									}
																																								}
																																								else {
																																									if (parameters[8].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], values_lists[7][i7], values_lists[8][i8], 0) == true) {
																																										if (num_parameters > 9) {
																																											for (int i9 = 0; i9 < size_of_values[9]; ++i9) {
																																												bool constraints_satisfied = true;
																																												for (int i = 0; i < num_parameters; i++) {
																																													if (parameters[i].constraint != NULL && parameters[i].constraint(values_lists[0][i0], values_lists[1][i1], values_lists[2][i2], values_lists[3][i3], values_lists[4][i4], values_lists[5][i5], values_lists[6][i6], values_lists[7][i7], values_lists[8][i8], values_lists[9][i9]) == false) {
																																														constraints_satisfied = false;
																																														break;
																																													}
																																												}
																																												if (constraints_satisfied == true) {
																																													put(serch_space_values, values_lists, num_parameters, i0, i1, i2, i3, i4, i5, i6, i7, i8, i9);
																																													serch_space_values->next = generate_ll(NULL);
																																													serch_space_values = serch_space_values->next;
																																													++serch_space_size;
																																												}
																																											}
																																										}
																																									}
																																								}
																																							}
																																						}
																																					}
																																				}
																																			}
																																		}
																																	}
																															}
																														}
																													}
																												}
																										}
																									}
																								}
																							}
																						
																					}
																				}
																			}
																		}
																	
																}
															}
														}
													}
												
											}
										}
									}
								}
							
						}
					}
				}
				
			}

			// Zugriff auf array ist schneller als auf linked_list (get_configuration)
			int** combinations = (int**)malloc(serch_space_size * sizeof(int*));
			int index = 0;
			while (current_ll->next) {
				combinations[index] = current_ll->value;
				++index;
				current_ll = current_ll->next;
			}

			search_space->combinations_array = combinations;
			search_space->size = serch_space_size;

			free(size_of_values);
			free(values);
			for (int i = 0; i < num_parameters; ++i) {
				free(values_lists[i]);
			}
			free(values_lists);

		}
}
//------------------------------------------------------------------------------------------

configuration_t get_config(search_space_t* search_space, int index) {

	tp_t** PTR = (tp_t**)malloc(search_space->size_of_combinations * sizeof(tp_t*));
	for (int i = 0; i < search_space->size_of_combinations; ++i) {
		PTR[i] = &(search_space->parameters[i]);
	}
	int* values = search_space->combinations_array[index];
	configuration_t result = {PTR,values,search_space->size_of_combinations };
	return result;
}
//------------------------------------------------------------------------------------------

void free_search_space(search_space_t* search_space) {
	printf("\nfree search space\n");
	//for(int i = 0; i < search_space->size; ++i){
		//free(search_space->combinations_array[i]); no need to free the values, they will be freed by deleting the list
	//}
	free(search_space->combinations_array);
	//free(search_space->parameters); // no need to free parameters, because they are being always allocated on the stack
	delete_ll(search_space->list);
	printf("free search space done\n");
}
//------------------------------------------------------------------------------------------

int indexused(int* used, int index, int size) {

	for (int i = 0; i < size; ++i) {
		if (used[i] == index) {
			return 1;
		}
	}
	return 0;
}
//------------------------------------------------------------------------------------------

bool finish(double time, int tested_conf, ABORT_TYPE abort_type) {

	if (abort_type == EVALUATIONS) {
		if (tested_conf >= 50) {
			return true;
		}
	}
	else if (abort_type == SECONDS) {
		if ((clock() - time) / 1000 >= 30) {
			return true;
		}
	}
	else if (abort_type == MINUTES) {
		if (((clock() - time) / 1000) / 60 >= 30) {
			return true;
		}
	}
	else if (abort_type == HOURS) {
		if (((clock() - time) / 1000) / 3600 >= 30) {
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------------------

void explore_search_space(search_space_t* search_space, cost_function_t cost_function,
	SEARCH_STRATEGY search_strategy, ABORT_TYPE abort_type, int abort_value,
	configuration_t* best_config, int* cost) {

	double begin = clock(); // in sec
	int a[10];
	int conf_count = search_space->size;
	int size_of_conf = search_space->size_of_combinations;
	int tested_conf_count = 0;
	
	if (search_strategy == EXHAUSTIVE) {
		*cost = 2147483647; // MAX_INT
		for (int i = 0; i < conf_count; i++) {
			configuration_t current_conf = get_config(search_space, i);

			for (int j = 0; j < size_of_conf; j++) {
				a[j] = current_conf.values[j];
			}

			int current_cost = cost_function(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
			tested_conf_count++;
			if (current_cost < *cost) {
				*cost = current_cost;
				*best_config = current_conf;
			}
			if (finish(begin, tested_conf_count, abort_type) == true) {
				break;
			}
		}
		return;
	}
	else { // RANDOM
		*cost = 2147483647; // MAX_INT
		int random_index;  
		int* tested_indices = (int*)malloc(conf_count * sizeof(int));
		int size_of_tested_indices = 0;
		for (int i = 0; i < conf_count; i++) {
			while (true) {
				random_index = rand() % conf_count;
				if (indexused(tested_indices, random_index, size_of_tested_indices) == 0) {
					tested_indices[size_of_tested_indices] = random_index;
					size_of_tested_indices++;
					break;
				}
				if (finish(begin, tested_conf_count, abort_type) == true) {
					free(tested_indices);
					return;
				}
			}
			configuration_t current_conf = get_config(search_space, random_index);
			for (int j = 0; j < size_of_conf; j++) {
				a[j] = current_conf.values[j];
			}

			int current_cost = cost_function(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
			tested_conf_count++;
			if (current_cost < *cost) {
				*cost = current_cost;
				*best_config = current_conf;
			}
			if (finish(begin, tested_conf_count, abort_type) == true) {
				break;
			}
		}
		free(tested_indices);
		return;
	}
}
