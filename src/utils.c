#include "../include/philosophers.h"

bool	ft_has_prioryty(t_tread_data *tr_data)
{
	int	i;
	int	cur;

	cur = tr_data->philo_index;
	i = 0;
	pthread_mutex_lock(&tr_data->data->general);
	while (i < tr_data->data->num_of_philos)
	{
		if (tr_data->data->philo[i].living > tr_data->data->philo[cur].living)
		{
			pthread_mutex_unlock(&tr_data->data->general);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(&tr_data->data->general);
	return (true);
}

void	ft_print_state2(t_data *data, long time_elapsed, int philo, char *msg)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->philo_died)
		printf("%ld %d %s\n", time_elapsed, philo + 1, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

void	ft_print_state(t_tread_data *tr_data, char *str)
{
	struct timeval	cur_time;
	long			time_elapsed;

	gettimeofday(&cur_time, NULL);
	time_elapsed = ft_timestamp(&tr_data->data->start_time, &cur_time);
	ft_print_state2(tr_data->data, time_elapsed, tr_data->philo_index, str);
}

long	ft_timestamp(struct timeval *start_time, struct timeval *cur_time)
{
	long	start;
	long	current;
	long	elapsed_time;

	start = (start_time->tv_sec * 1000000) + start_time->tv_usec;
	current = (cur_time->tv_sec * 1000000) + cur_time->tv_usec;
	elapsed_time = (current - start) / 1000;
	return (elapsed_time);
}
