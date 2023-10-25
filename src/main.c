#include "structs.h"

// #define DEBUG

int main(int argc, char const *argv[])
{
	int sensor_numbers;

	FILE *in = fopen(argv[1], "r");

	if (!in) {
		printf("Malloc failed\n");
		return 0;
	}

	fread(&sensor_numbers, sizeof(int), 1, in);

	// printf("%d\n", sensor_numbers);

	sensor **sensor = malloc(sensor_numbers * sizeof(struct sensor *));


	for (int i = 0; i < sensor_numbers; i++) {
		sensor[i] = alloc_sensor(in);
	}

	// sortare: punem pmu la inceputul vectorului
	qsort(sensor, sensor_numbers, sizeof(struct sensor *), compare_sensors);

#ifdef DEBUG
	printf("%d\n", sensor_numbers);
	// enum sensor_type = sensor[0].sensor_type;
	// printf("%d", sensor[0].sensor_type);
	for (int i = 0; i < sensor_numbers; i++) {
		printf("%d\n", i);
		if (sensor[i].sensor_type == TIRE) {
			printf("Tire sensors:\n");
		} else {
			printf("Power management unit sensors:\n");
		}
	}
#endif

	// char line[MAX_STRING_SIZE];

	while (1) {
		char command[MAX_STRING_SIZE];
		scanf("%s", command);

		if (strcmp(command, "print") == 0) {
			int sensor_idx;
			scanf("%d", &sensor_idx);

			if (sensor_idx < 0 || sensor_idx >= sensor_numbers) {
				printf("Index not in range!\n");
				continue;
			}
			print_sensor(sensor[sensor_idx]);

		} else if (strcmp(command, "analyze") == 0) {
			int sensor_idx;
			scanf("%d", &sensor_idx);

			if (sensor_idx < 0 || sensor_idx >= sensor_numbers) {
				printf("Index not in range!\n");
				continue;
			}


			void *operations[8] = {NULL};
			get_operations(operations);

			for (int i = 0; i < sensor[sensor_idx]->nr_operations; i++) {

				((void (*)(void *)) operations[sensor[sensor_idx]->
				operations_idxs[i]])(sensor[sensor_idx]->sensor_data);
			}

		} else if (strcmp(command, "clear") == 0) {

			int ok, nr = sensor_numbers;
			for (int i = 0; i < sensor_numbers; i++) {
				ok = clear_sensor(sensor[i], &sensor_numbers);
				if (!ok) {
					// Rearanjare vector
					for (int j = i + 1; j < nr; j++) {
						*(sensor + j - 1) = *(sensor + j);
					}
					i--;
				}
			}
			sensor = realloc(sensor, sensor_numbers * sizeof(struct sensor *));

		} else if (strcmp(command, "exit") == 0) {
			for (int i = 0; i < sensor_numbers; i++) {
				dealloc_sensor(sensor[i]);
			}
			free(sensor);
			fclose(in);

			return 0;

		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
