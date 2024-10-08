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

bool ft_has_prioryty(tread_data_t *tr_data)
{
	int i;
	int cur;
	cur = tr_data->philo_index;
	i = 0;
	pthread_mutex_lock(&tr_data->data->general);
	while(i < tr_data->data->num_of_philos)
	{
		if(tr_data->data->philo[i].living > tr_data->data->philo[cur].living)
		{
			pthread_mutex_unlock(&tr_data->data->general);
			return false;
		}
		i++;
	}
	pthread_mutex_unlock(&tr_data->data->general);
	return true;
}

void ft_print_state(data_t *data, long time_elapsed, int philo, char *msg)
{	
	pthread_mutex_lock(&data->print_mutex);
	if(!data->philo_died)  // big problem here with race condition
	{
		printf("%ld %d %s\n", time_elapsed, philo + 1, msg);
	}
	pthread_mutex_unlock(&data->print_mutex);
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