#include "../include/philosophers.h"

void	ft_eating(t_tread_data *tread, int l_fork, int r_fork)
{
	struct timeval	start_eat;
	long			time;

	while (1)
	{
		if (tread->data->forks[l_fork] == 0 && tread->data->forks[r_fork] == 0)
		{
			tread->data->forks[l_fork] = 1;
			tread->data->forks[r_fork] = 1;
			tread->data->philo[tread->philo_index].cur_state = EAT;
			tread->data->philo[tread->philo_index].times_eaten++;
			gettimeofday(&start_eat, NULL);
			time = ft_timestamp(&tread->data->start_time, &start_eat);
			pthread_mutex_lock(&tread->data->general);
			tread->data->philo[tread->philo_index].last_meal = time;
			pthread_mutex_unlock(&tread->data->general);
			ft_print_state2(tread->data, time, tread->philo_index, "is eating");
			usleep(tread->data->time_to_eat * TIME_UNIT);
			tread->data->forks[l_fork] = 0;
			tread->data->forks[r_fork] = 0;
			tread->data->philo[tread->philo_index].cur_state = SLEEP;
			ft_print_state(tread, "is sleeping");
			break ;
		}
	}
}

void	ft_sleeping(t_tread_data *tr_data)
{
	usleep(tr_data->data->time_to_sleep * TIME_UNIT);
	tr_data->data->philo[tr_data->philo_index].cur_state = THINK;
	ft_print_state(tr_data, "is thinking");
}
