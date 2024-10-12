#include "../include/philosophers.h"

void	ft_fork_indexing(t_tread_data *tr_data)
{
	int	i;

	i = tr_data->philo_index;
	tr_data->data->philo[i].right_fork = i;
	if (i == 0)
		tr_data->data->philo[i].left_fork = tr_data->data->num_of_philos - 1;
	else
		tr_data->data->philo[i].left_fork = i - 1;
}

void	ft_fork_acquisition(t_tread_data *tr_data, int left_fork,
		int right_fork)
{
	if (left_fork < right_fork)
	{
		pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
		ft_print_state(tr_data, "has taken a fork");
		pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
		ft_print_state(tr_data, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
		ft_print_state(tr_data, "has taken a fork");
		pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
		ft_print_state(tr_data, "has taken a fork");
	}
}

void	*ft_philo_routine(void *arg)
{
	t_tread_data	*tr_data;
	int				left_fork;
	int				right_fork;

	tr_data = (t_tread_data *)arg;
	ft_fork_indexing(tr_data);
	left_fork = tr_data->data->philo[tr_data->philo_index].left_fork;
	right_fork = tr_data->data->philo[tr_data->philo_index].right_fork;
	while (!ft_death(tr_data) && !ft_all_feed(tr_data))
	{
		if (tr_data->data->philo[tr_data->philo_index].cur_state == THINK
			&& ft_has_prioryty(tr_data))
		{
			ft_fork_acquisition(tr_data, left_fork, right_fork);
			ft_eating(tr_data, left_fork, right_fork);
			pthread_mutex_unlock(&tr_data->data->fork_mutexes[left_fork]);
			pthread_mutex_unlock(&tr_data->data->fork_mutexes[right_fork]);
		}
		if (tr_data->data->philo[tr_data->philo_index].cur_state == SLEEP)
			ft_sleeping(tr_data);
		usleep(100);
	}
	return (NULL);
}
