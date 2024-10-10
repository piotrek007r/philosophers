#include "../include/philosophers.h"

void ft_destroy_mutex(data_t *data)
{
	int i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_destroy(&data->fork_mutexes[i]) != 0)
		{
			perror("Failed to destroy mutex");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	if(pthread_mutex_destroy(&data->print_mutex) != 0)
	{
		perror("Failed to destroy mutex");
		exit(EXIT_FAILURE);
	}
	if(pthread_mutex_destroy(&data->general) != 0)
	{
		perror("Failed to destroy mutex");
		exit(EXIT_FAILURE);
	}
}



void ft_close(data_t *data, pthread_t *th, tread_data_t **tr_data)
{
	int i;

	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	ft_destroy_mutex(data);
	ft_free_allocs(data, tr_data);
}