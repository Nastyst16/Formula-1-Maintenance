#include "structs.h"

// ------- DO NOT MODIFY ------- //

static void tire_pressure_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->pressure >= 21 && t->pressure <= 26) {
		printf("Tire has normal pressure.\n");
	} else if (t->pressure > 26 && t->pressure <= 28) {
		printf("Tire has high pressure.\n");
	} else if (t->pressure >= 19 && t->pressure < 21) {
		printf("Tire has low pressure.\n");
	} else {
		printf("Tire has abnormal pressure.\n");
	}
}

static void tire_temperature_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->temperature >= 0 && t->temperature <= 120) {
		printf("Tire has normal temperature.\n");
	} else {
		printf("Tire has abnormal temperature.\n");
	}
}

static void tire_wear_level_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->wear_level >= 0 && t->wear_level <= 20) {
		printf("Tire is new.\n");
	} else if (t->wear_level >= 20 && t->wear_level <= 40) {
		printf("Tire is slightly used.\n");
	} else if (t->wear_level >= 40 && t->wear_level <= 60) {
		printf("Tire is used. Consider a pit stop!\n");
	} else {
		printf("Tire is extremely used. Box this lap!\n");
	}
}

static void tire_performance_score(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	int score = 0;

	// Check pressure
	if (t->pressure >= 21 && t->pressure <= 26) {
		score += 4;
	} else if (t->pressure >= 19 && t->pressure < 21) {
		score += 2;
	} else if (t->pressure > 26 && t->pressure <= 28) {
		score += 3;
	} else {
		score -= 1;
	}

	// Check temperature
	if (t->temperature >= 80 && t->temperature <= 100) {
		score += 4;
	} else if (t->temperature >= 100 && t->temperature < 120) {
		score += 3;
	} else if (t->temperature > 60 && t->temperature <= 80) {
		score += 2;
	} else {
		score -= 1;
	}

	// Check wear level
	if (t->wear_level >= 0 && t->wear_level < 20) {
		score += 4;
	} else if (t->wear_level >= 20 && t->wear_level < 40) {
		score += 2;
	} else if (t->wear_level > 40 && t->wear_level < 60) {
		score -= 1;
	} else {
		score -= 2;
	}

	// Cap the score at 10
	if (score > 10) {
		score = 10;
	} else if (score < 1) {
		score = 1;
	}

	t->performace_score = score;
	printf("The tire performance score is: %d\n", score);
}

static void pmu_compute_power(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float power = pmu->voltage * pmu->current;

	printf("Power output: %.2f kW.\n", power);
}

static void pmu_regenerate_energy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;

	pmu->energy_storage += pmu->energy_regen;
	pmu->energy_regen = 0;

	// Cap the energy storage to 100%
	if (pmu->energy_storage > 100)
		pmu->energy_storage = 100;

	printf("Energy left in storage: %d\n", pmu->energy_storage);
}

static void pmu_get_energy_usage(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float energy = pmu->power_consumption * pmu->current;

	printf("Energy usage: %.2f kW\n", energy);
}

static void pmu_is_battery_healthy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	int ok = 0;

	if (pmu->voltage < 10 || pmu->voltage > 20) {
		printf("Battery voltage out of range: %.2fV\n", pmu->voltage);
		ok = 1;
	}

	if (pmu->current < -100 || pmu->current > 100) {
		printf("Battery current out of range: %.2fA\n", pmu->current);
		ok = 1;
	}

	if (pmu->power_consumption < 0 || pmu->power_consumption > 1000) {
		printf("Power consumption out of range: %.2f kW\n",
			   pmu->power_consumption);
		ok = 1;
	}

	if (pmu->energy_regen < 0 || pmu->energy_regen > 100) {
		printf("Energy regeneration out of range: %d%%\n", pmu->energy_regen);
		ok = 1;
	}

	if (pmu->energy_storage < 0 || pmu->energy_storage > 100) {
		printf("Energy storage out of range: %d%%\n", pmu->energy_storage);
		ok = 1;
	}

	if (ok == 1)
		return;

	printf("Battery working as expected!\n");
}

void get_operations(void **operations)
{
	operations[0] = tire_pressure_status;
	operations[1] = tire_temperature_status;
	operations[2] = tire_wear_level_status;
	operations[3] = tire_performance_score;
	operations[4] = pmu_compute_power;
	operations[5] = pmu_regenerate_energy;
	operations[6] = pmu_get_energy_usage;
	operations[7] = pmu_is_battery_healthy;
}

// ------- DO NOT MODIFY ------- //

// Definirea functiei de comparare

int compare_sensors(const void *a, const void *b)
{
	sensor **sensor_a = (sensor **)a;
	sensor **sensor_b = (sensor **)b;

	if ((*sensor_a)->sensor_data == NULL) {
		return 1;
	} else if ((*sensor_a)->sensor_type == TIRE &&
			   (*sensor_b)->sensor_type == PMU) {
		return 1;
	} else if ((*sensor_a)->sensor_type == PMU &&
			   (*sensor_b)->sensor_type == TIRE) {
		return -1;
	} else {
		return 0;
	}
}

sensor *alloc_sensor(FILE *in)
{
	sensor *sensor = malloc(sizeof(*sensor));

	int sensor_type;
	fread(&sensor_type, sizeof(int), 1, in);
	if (sensor_type == 0)
		sensor->sensor_type = TIRE;
	else if (sensor_type == 1)
		sensor->sensor_type = PMU;

	if (sensor_type == 1) {
		power_management_unit *pmu_data = malloc(sizeof(power_management_unit));

		fread(pmu_data, sizeof(power_management_unit), 1, in);
		sensor->sensor_data = (void *)pmu_data;

	} else {
		tire_sensor *tire_data = malloc(sizeof(tire_sensor));

		fread(tire_data, sizeof(tire_sensor), 1, in);
		sensor->sensor_data = (void *)tire_data;
	}

	fread(&sensor->nr_operations, sizeof(int), 1, in);

	sensor->operations_idxs = malloc(sensor->nr_operations * sizeof(int));
	fread(sensor->operations_idxs, sizeof(int), sensor->nr_operations, in);

	return sensor;
}


void print_sensor(sensor *s)
{
	if (s->sensor_type == PMU) {
		power_management_unit *pmu_data =
		(power_management_unit *)s->sensor_data;

		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n", pmu_data->voltage);
		printf("Current: %.2f\n", pmu_data->current);
		printf("Power Consumption: %.2f\n", pmu_data->power_consumption);
		printf("Energy Regen: %d%%\n", pmu_data->energy_regen);
		printf("Energy Storage: %d%%\n", pmu_data->energy_storage);
	} else {
		tire_sensor *tire_data = (tire_sensor *)s->sensor_data;
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n", tire_data->pressure);
		printf("Temperature: %.2f\n", tire_data->temperature);
		printf("Wear Level: %d%%\n", tire_data->wear_level);
		if (tire_data->performace_score == 0)
			printf("Performance Score: Not Calculated\n");
		else
			printf("Performance Score: %d\n", tire_data->performace_score);
	}
}

int clear_sensor(sensor *s, int *sensor_numbers)
{
	if (s->sensor_type == PMU) {
		power_management_unit *pmu_data =
		(power_management_unit *)s->sensor_data;

		if (pmu_data->voltage < 10 || pmu_data->voltage > 20 ||
			pmu_data->current < -100 || pmu_data->current > 100 ||
			pmu_data->power_consumption < 0 ||
			pmu_data->power_consumption > 1000 ||
			pmu_data->energy_regen < 0 || pmu_data->energy_regen > 100 ||
			pmu_data->energy_storage < 0 || pmu_data->energy_storage > 100) {

			free(pmu_data);
			free(s->operations_idxs);
			free(s);
			--(*sensor_numbers);
			return 0;
		}

	} else {
		tire_sensor *tire_data = (tire_sensor *)s->sensor_data;
		if (tire_data->pressure < 19 || tire_data->pressure > 28 ||
			tire_data->temperature < 0 || tire_data->temperature > 120 ||
			tire_data->wear_level < 0 || tire_data->wear_level > 100) {

			free(tire_data);
			free(s->operations_idxs);
			free(s);
			--(*sensor_numbers);

			return 0;
		}
	}
	return 1;
}

void dealloc_sensor(sensor *s)
{
	if (s->sensor_type == PMU) {
		power_management_unit *pmu_data =
		(power_management_unit *)s->sensor_data;

		free(pmu_data);
	} else {
		tire_sensor *tire_data = (tire_sensor *)s->sensor_data;
		free(tire_data);
	}
	free(s->operations_idxs);
	free(s);
}

