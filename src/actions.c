#include "../include/philosophers.h"

bool ft_death(data_t *data)
{
	int i;

	i = 0;
	pthread_mutex_lock(&data->print_mutex);
	while(i < data->num_of_philos)
	{
		if(data->philo[i].has_died)
		{
			pthread_mutex_unlock(&data->print_mutex);
			return true;
		}
		i++;
	}
	pthread_mutex_unlock(&data->print_mutex);
	return false;
}