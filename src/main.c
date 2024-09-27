#include "../include/philosophers.h"

void	ft_tester(data_t *data, int key)
{
	if (key == 1)
	{
		for (int i = 0; i < data->num_of_philos; i++)
		{
			printf("philo %d, current status: %d eating: %d, sleeping %d, living %d\n", i + 1, data->philo[i].current_state,
				data->philo[i].eating, data->philo[i].sleeping,
				data->philo[i].living);
		}
		printf("-------------------------------\n");
	}
	if (key == 2)
	{
		for (int i = 0; i < data->num_of_philos; i++)
		{
			printf("key num %d is: %d\n", i, data->forks[i]);
		}
	}
}

void	ft_eating(tread_data_t *tread_data, int left_fork, int right_fork)
{
	while (1)
	{
		if (tread_data->data->forks[left_fork] == 0
			&& tread_data->data->forks[right_fork] == 0)
		{
			tread_data->data->forks[left_fork] = 1;
			tread_data->data->forks[right_fork] = 1;
			// printf("**philo nr %d has pick up forks: %d and %d\n",tread_data->philo_index,
			//     left_fork, right_fork);
			// printf("++philo nr %d start eating\n", tread_data->philo_index);
			usleep(tread_data->data->time_to_eat * 10000);
			tread_data->data->forks[left_fork] = 0;
			tread_data->data->forks[right_fork] = 0;
			// printf("**philo nr %d has put away forks: %d and %d\n",tread_data->philo_index,
			//     left_fork, right_fork);
			// printf("++philo nr %d finish eating\n", tread_data->philo_index);
			tread_data->data->philo[tread_data->philo_index].has_eaten = 1;
			tread_data->data->philo[tread_data->philo_index].current_state = SLEEP;
			break ;
		}
		usleep(100); // adjust this
	}
}

void	*ft_philo_routine(void *arg)
{
	tread_data_t	*tread_data;
	int				i;
	int				left_fork;
	int				right_fork;

	tread_data = (tread_data_t *)arg;
	i = 0;
	right_fork = tread_data->philo_index;
	if (tread_data->philo_index == 0)
		left_fork = tread_data->data->num_of_philos - 1;
	else
		left_fork = tread_data->philo_index - 1;
	while (1)
	{
		if (tread_data->data->philo[tread_data->philo_index].current_state == THINK)
		{
			tread_data->data->philo[tread_data->philo_index].current_state = EAT;
			if (left_fork < right_fork)
			{
				pthread_mutex_lock(&tread_data->data->fork_mutexes[left_fork]);
				pthread_mutex_lock(&tread_data->data->fork_mutexes[right_fork]);
			}
			else
			{
				pthread_mutex_lock(&tread_data->data->fork_mutexes[right_fork]);
				pthread_mutex_lock(&tread_data->data->fork_mutexes[left_fork]);
			}
			ft_eating(tread_data, left_fork, right_fork);

			pthread_mutex_unlock(&tread_data->data->fork_mutexes[left_fork]);
			pthread_mutex_unlock(&tread_data->data->fork_mutexes[right_fork]);
			printf("unlocking fork: %d and %d\n", left_fork, right_fork);

		}
		if (tread_data->data->philo[tread_data->philo_index].current_state == SLEEP)
		{
			printf("--philo nr %d take a nap\n", tread_data->philo_index);
			usleep(tread_data->data->time_to_sleep * 10000); // Sleeping time
			printf("--philo nr %d has wake\n", tread_data->philo_index);
			tread_data->data->philo[tread_data->philo_index].current_state = THINK;
		}
		// i++;
		// if(i > 0)
		//     break ;
	}
}

void	ft_global_clock(data_t *data)
{
	int	i;
	int	temp;
	int	seconds;

	seconds = 0;
	while (1)
	{
		usleep(1000000);
		i = 0;
		temp = 0;
		while (i < data->num_of_philos)
		{
			if (data->philo[i].has_eaten == 1)
			{
				printf("has_eaten!!\n");
				data->philo[i].has_eaten = 0;
				data->philo[i].living = 0;
			}
			// if(data->philo[i].current_state == 3)
			//     temp++;
			// if(temp == data->num_of_philos)
			// {
			//     printf("END!!!\n");
			//     exit(0);
			// }
			data->philo[i].living++;
			if (data->philo[i].living == data->time_to_die)
				printf("Die motherfucker!\n");
			i++;
		}
		ft_tester(data, 1);
		seconds++;
		printf("seconds passed: %d\n", seconds);
		if (seconds > 20)
			break ;
	}
}

void	ft_init(data_t *data)
{
	int				i;
	pthread_t		th[data->num_of_philos];
	tread_data_t	*tread_data[data->num_of_philos];
	pthread_mutex_t	*mutexes;

	data->fork_mutexes = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
	i = 0;
	while (i < data->num_of_philos)
	{
		if (pthread_mutex_init(&data->fork_mutexes[i], NULL) != 0)
		{
			perror("Failed to initialize mutex");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < data->num_of_philos)
	{
		tread_data[i] = malloc(sizeof(tread_data_t)); // to free
		tread_data[i]->philo_index = i;              
			// here change philo numbering from 0 to 1
		tread_data[i]->data = data;
		if (pthread_create(&th[i], NULL, &ft_philo_routine, tread_data[i]) != 0)
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
		usleep(100);
		i++;
	}
	ft_global_clock(data);
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
}

void	ft_create_philos(data_t *data, char **argv)
{
	int	i;

	data->num_of_philos = ft_atoi(argv[1]);
	data->philo = ft_calloc(data->num_of_philos, sizeof(philo_t));     
		// to free
	data->forks = ft_calloc(data->num_of_philos, sizeof(int *));       
		// to free
	data->philo_states = ft_calloc(data->num_of_philos, sizeof(int *));
		// to free
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	i = 0;
	while (i < data->num_of_philos)
	{
		data->philo[i].philo_num = i + 1;
		data->philo[i].current_state = THINK;
		i++;
	}
}

int	main(int argc, char **argv)
{
	data_t	*data;

	setbuf(stdout, NULL); // Remove this !!!
	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(data_t)); // to free
		if (argc == 6)
			data->num_of_meals = ft_atoi(argv[6]);
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

To free:
1: data = malloc(sizeof(data_t));
2: data->philo = malloc(sizeof(philo_t) * data->num_of_philos); // to free
3: data->forks = ft_calloc(data->num_of_philos, sizeof(int *)
		* data->num_of_philos); // to free
4: data->philo_states = ft_calloc(data->num_of_philos, sizeof(int *));
	// to free
5: tread_data[i] = malloc(sizeof(tread_data_t));
6: data->fork_mutexes = malloc(data->num_of_philos * sizeof(pthread_mutex_t));
*/

/*
GARBAGE

		// data->philo[i].living++;
		// if(data->philo_states[i] == 0)
		//     data->philo[i].eating++;
		// if(data->philo_states[i] == 1)
		//     data->philo[i].sleeping++;
		// if(data->philo[i].living == data->time_to_die)
		//     printf("Die motherfucker!\n");
		// if(data->philo[i].eating == data->time_to_eat)
		// {
		//     data->philo[i].eating = 0;
		//     data->philo[i].living = 0;
		//     data->philo_states[i] = 1;
		//     data->philo[i].status_change = 1;
		// }
		// if(data->philo[i].sleeping == data->time_to_sleep)
		// {
		//     data->philo[i].sleeping = 0;
		//     data->philo_states[i] = 2;
		//     data->philo[i].status_change = 1;
		// }

		// it can prevent deadlock
					// if (right_fork < left_fork)
			// {
			//     pthread_mutex_lock(&tread_data->data->fork_mutexes[right_fork]);
			//     pthread_mutex_lock(&tread_data->data->fork_mutexes[left_fork]);
			// }
			// else
			// {
			//     pthread_mutex_lock(&tread_data->data->fork_mutexes[left_fork]);
			//     pthread_mutex_lock(&tread_data->data->fork_mutexes[right_fork]);
			// }
*/