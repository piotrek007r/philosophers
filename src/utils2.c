/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pruszkie <pruszkie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:49:51 by pruszkie          #+#    #+#             */
/*   Updated: 2024/10/16 14:49:53 by pruszkie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

bool	ft_death(t_tread_data *tr_data)
{
	pthread_mutex_lock(&tr_data->data->general);
	if (tr_data->data->philo_died)
	{
		pthread_mutex_unlock(&tr_data->data->general);
		return (true);
	}
	pthread_mutex_unlock(&tr_data->data->general);
	return (false);
}

bool	ft_all_feed(t_tread_data *tr_data)
{
	if (tr_data->data->num_of_meals > 0)
	{
		if (tr_data->data->philo[tr_data->philo_index].times_eaten
			== tr_data->data->num_of_meals)
		{
			pthread_mutex_lock(&tr_data->data->general);
			tr_data->data->philo[tr_data->philo_index].is_feed = true;
			pthread_mutex_unlock(&tr_data->data->general);
			return (true);
		}
	}
	return (false);
}

int	ft_atoi(const char *str)
{
	int	isminus;
	int	result;

	isminus = 1;
	result = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			isminus = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + *str - '0';
		str++;
	}
	return (result * isminus);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*temp;
	size_t			counter;

	if (size == 0 || nmemb == 0)
	{
		size = 1;
		nmemb = 1;
	}
	if (size > 2147483647 / nmemb)
	{
		return (NULL);
	}
	temp = (void *)malloc(nmemb * size);
	if (!temp)
		return (NULL);
	counter = 0;
	while ((nmemb * size) > counter)
	{
		temp[counter] = 0;
		counter++;
	}
	return (temp);
}
