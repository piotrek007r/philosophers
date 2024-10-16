/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pruszkie <pruszkie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:48:59 by pruszkie          #+#    #+#             */
/*   Updated: 2024/10/16 14:49:02 by pruszkie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	ft_destroy_mutex(t_data *data)
{
	int	i;

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
	if (pthread_mutex_destroy(&data->print_mutex) != 0)
	{
		perror("Failed to destroy mutex");
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_destroy(&data->general) != 0)
	{
		perror("Failed to destroy mutex");
		exit(EXIT_FAILURE);
	}
}

void	ft_free_allocs(t_data *data, t_tread_data **tr_data)
{
	int	i;

	i = 0;
	free(data->fork_mutexes);
	free(data->forks);
	free(data->philo_states);
	free(data->philo);
	while (i < data->num_of_philos)
	{
		free(tr_data[i]);
		i++;
	}
	free(data);
}

void	ft_close(t_data *data, pthread_t *th, t_tread_data **tr_data)
{
	int	i;

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
