#include "../include/philosophers.h"

void	ft_tester(data_t *data, int key)
{
	if (key == 1)
	{
		for (int i = 0; i < data->num_of_philos; i++)
		{
			printf("philo %d, current status: %d eated: %d living %d\n", i + 1, data->philo[i].cur_state,
				data->philo[i].times_eaten,
				data->philo[i].living);
		}
		printf("-------------------------------\n");
	}
	if (key == 2)
	{
		for (int i = 0; i < data->num_of_philos; i++)
		{
			printf("key num %d is: %d\n", i, data->forks[i]);
		}
	}
}

long ft_timestamp(struct timeval *start_time, struct timeval *cur_time)
{
	long start;
	long current;
	long elapsed_time;

	start = (start_time->tv_sec * 1000000) + start_time->tv_usec;
	current = (cur_time->tv_sec * 1000000) + cur_time->tv_usec;
	elapsed_time = (current - start) / 1000;
	return elapsed_time;
} 