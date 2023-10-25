#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_STRING_SIZE 128

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

enum sensor_type {
	TIRE,
	PMU
};

typedef struct {
	enum sensor_type sensor_type;
	void *sensor_data;
	int nr_operations;
	int *operations_idxs;
} sensor;

typedef struct {
	float voltage;
	float current;
	float power_consumption;
	int energy_regen;
	int energy_storage;
} power_management_unit;

typedef struct {
	float pressure;
	float temperature;
	int wear_level;
	int performace_score;
} tire_sensor;

#endif // _STRUCTS_H_

static void tire_pressure_status(void *data);
static void tire_temperature_status(void *data);
static void tire_wear_level_status(void *data);
static void tire_performance_score(void *data);
static void pmu_compute_power(void *data);
static void pmu_regenerate_energy(void *data);
static void pmu_get_energy_usage(void *data);
static void pmu_is_battery_healthy(void *data);

int compare_sensors(const void *a, const void *b);
void get_operations(void **operations);
int clear_sensor(sensor *s, int *sensor_numbers);

sensor *alloc_sensor(FILE *in);
void print_sensor(sensor *s);
void dealloc_sensor(sensor *s);