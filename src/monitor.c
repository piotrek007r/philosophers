#include "../include/philosophers.h"

long	ft_monitor_check_meals(t_data *data, long time_elapsed, int i)
{
	long	meal_times;

	pthread_mutex_lock(&data->general);
	meal_times = time_elapsed - data->philo[i].last_meal;
	data->philo[i].living = meal_times;
	if (!data->philo[i].is_feed)
		data->all_feed = false;
	pthread_mutex_unlock(&data->general);
	return (meal_times);
}

void	ft_monitor_check_death(t_data *data, long meal_times, long time_elapsed,
		int i)
{
	struct timeval	death_time;

	pthread_mutex_lock(&data->death_mutex);
	if (meal_times > data->time_to_die && !data->philo[i].is_feed
		&& !data->philo_died)
	{
		gettimeofday(&death_time, NULL);
		time_elapsed = ft_timestamp(&data->start_time, &death_time);
		data->philo[i].has_died = true;
		ft_print_state2(data, time_elapsed, i, "died");
		data->philo_died = true;
	}
	pthread_mutex_unlock(&data->death_mutex);
}

long	ft_monitor_iteration(t_data *data)
{
	struct timeval	cur_time;

	usleep(10);
	gettimeofday(&cur_time, NULL);
	return (ft_timestamp(&data->start_time, &cur_time));
}

void	ft_monitor(t_data *data)
{
	int		i;
	long	time_elapsed;
	long	meal_times;

	while (1)
	{
		time_elapsed = ft_monitor_iteration(data);
		data->all_feed = true;
		i = 0;
		while (i < data->num_of_philos)
		{
			meal_times = ft_monitor_check_meals(data, time_elapsed, i);
			ft_monitor_check_death(data, meal_times, time_elapsed, i);
			i++;
		}
		if (data->all_feed && (data->num_of_meals > 0))
			break ;
		pthread_mutex_lock(&data->death_mutex);
		if (data->philo_died)
			break ;
		pthread_mutex_unlock(&data->death_mutex);
	}
}
