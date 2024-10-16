/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pruszkie <pruszkie@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 14:49:14 by pruszkie          #+#    #+#             */
/*   Updated: 2024/10/16 14:49:16 by pruszkie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	ft_mutexes_init(t_data *data)
{
	int	i;
	int	raport;

	data->fork_mutexes = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
	i = 0;
	raport = pthread_mutex_init(&data->general, NULL);
	raport = pthread_mutex_init(&data->print_mutex, NULL);
	raport = pthread_mutex_init(&data->death_mutex, NULL);
	while (i < data->num_of_philos)
	{
		raport = pthread_mutex_init(&data->fork_mutexes[i], NULL);
		i++;
	}
	if (raport != 0)
	{
		perror("Failed to initialize fork_mutex");
		exit(EXIT_FAILURE);
	}
}

void	ft_init(t_data *data)
{
	int				i;
	pthread_t		th[200];
	t_tread_data	*tread_data[200];

	ft_mutexes_init(data);
	i = 0;
	while (i < data->num_of_philos)
	{
		tread_data[i] = malloc(sizeof(t_tread_data));
		tread_data[i]->philo_index = i;
		tread_data[i]->data = data;
		if (pthread_create(&th[i], NULL, &ft_philo_routine, tread_data[i]) != 0)
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
		usleep(100);
		i++;
	}
	ft_monitor(data);
	ft_close(data, th, tread_data);
}

void	ft_one_philo_case(t_data *data, int time_to_die)
{
	if (data->num_of_philos == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(time_to_die * TIME_UNIT);
		printf("%d 1 died\n", time_to_die + 1);
		free(data);
		exit(0);
	}
}

void	ft_create_philos(t_data *data, char **argv)
{
	int	i;

	data->num_of_philos = ft_atoi(argv[1]);
	ft_one_philo_case(data, ft_atoi(argv[2]));
	data->philo = ft_calloc(data->num_of_philos, sizeof(t_philo));
	data->forks = ft_calloc(data->num_of_philos, sizeof(int *));
	data->philo_states = ft_calloc(data->num_of_philos, sizeof(int *));
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->philo_died = false;
	i = 0;
	while (i < data->num_of_philos)
	{
		data->philo[i].philo_num = i + 1;
		data->philo[i].cur_state = THINK;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data			*data;
	struct timeval	start_time;

	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(t_data));
		gettimeofday(&start_time, NULL);
		data->start_time = start_time;
		if (argc == 6)
			data->num_of_meals = ft_atoi(argv[5]);
		else
			data->num_of_meals = 0;
		ft_create_philos(data, argv);
		ft_init(data);
	}
}

/*
Args:
1 - number_of_philosophers
2 - time_to_die
3 - time_to_eat
4 - time_to_sleep
5 - number_of_times_each_philosopher_must_eat

States:

0 - eat
1 - sleep
2 - thinking
*/
// setbuf(stdout, NULL); // Remove this !!!
