#include "../include/philosophers.h"

void	ft_eating(tread_data_t *tr_data, int left_fork, int right_fork)
{
	struct timeval start_eat;
	long time_elapsed;

	while (1)
	{
		if (tr_data->data->forks[left_fork] == 0 && tr_data->data->forks[right_fork] == 0)
		{
			tr_data->data->forks[left_fork] = 1;
			tr_data->data->forks[right_fork] = 1;
			tr_data->data->philo[tr_data->philo_index].cur_state = EAT;
			tr_data->data->philo[tr_data->philo_index].times_eaten++;
			gettimeofday(&start_eat, NULL);
			time_elapsed = ft_timestamp(&tr_data->data->start_time, &start_eat);
			pthread_mutex_lock(&tr_data->data->general);
			tr_data->data->philo[tr_data->philo_index].last_meal = time_elapsed;
			pthread_mutex_unlock(&tr_data->data->general);
			ft_print_state2(tr_data->data, time_elapsed, tr_data->philo_index, "is eating");
			usleep(tr_data->data->time_to_eat * TIME_UNIT);
			tr_data->data->forks[left_fork] = 0;
			tr_data->data->forks[right_fork] = 0;
			tr_data->data->philo[tr_data->philo_index].cur_state = SLEEP;
			ft_print_state(tr_data, "is sleeping"); // might try to move to ft_sleeping
			break;
		}
	}
}

void ft_sleeping(tread_data_t *tr_data)
{
			usleep(tr_data->data->time_to_sleep * TIME_UNIT);
			tr_data->data->philo[tr_data->philo_index].cur_state = THINK;
			ft_print_state(tr_data, "is thinking");
}