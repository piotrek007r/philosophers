#include "../include/philosophers.h"

void ft_mutexes_init(data_t *data)
{
	int i;
	int raport;

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
	if(raport != 0)
	{
		perror("Failed to initialize fork_mutex");
		exit(EXIT_FAILURE);
	}
}

void	ft_init(data_t *data)
{
	int				i;
	pthread_t		th[data->num_of_philos];
	tread_data_t	*tread_data[data->num_of_philos];

	ft_mutexes_init(data);
	i = 0;
	while (i < data->num_of_philos)
	{
		tread_data[i] = malloc(sizeof(tread_data_t)); // to free
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

void	ft_create_philos(data_t *data, char **argv)
{
	int	i;

	data->num_of_philos = ft_atoi(argv[1]);
	data->philo = ft_calloc(data->num_of_philos, sizeof(philo_t));     
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
	data_t	*data;
	struct timeval start_time;

	// setbuf(stdout, NULL); // Remove this !!!
	if (argc == 5 || argc == 6)
	{
		data = malloc(sizeof(data_t)); // to free
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
// long ft_timepassed(struct timeval start, struct timeval end)
// {
// 	long seconds_diff = end.tv_sec - start.tv_sec;
//     long useconds_diff = end.tv_usec - start.tv_usec;

//     return (seconds_diff * 1000000) + useconds_diff;
// }

// bool ft_relaxed_priority(tread_data_t *tr_data) {
//     int i;
//     int minimum = tr_data->data->philo[tr_data->philo_index].living;
//     int skip_count = 0; // Count how many times the philosopher has been skipped

//     // Check how many philosophers have lower living values
//     for (i = 0; i < tr_data->data->num_of_philos; i++) {
//         if (tr_data->data->philo[i].living < minimum) {
//             skip_count++;
//         }
//     }

//     // If this philosopher is skipped more than a threshold (e.g., 2), allow them to eat
//     if (skip_count >= 2) {
//         return true; // Let this philosopher eat
//     }

//     // Allow the philosopher to eat if they have the lowest living value
//     return (skip_count == 0);
// }

// void	ft_global_clock(data_t *data)
// {
// 	int	i;
// 	int	temp;
// 	int	seconds;
// 	int all_feed;
// 	struct timeval death_time;
// 	long time_elapsed;

// 	data->cur_time = 0;
// 	all_feed = false;
// 	while (1)
// 	{
//   	// printf("seconds : %ld\nmicro seconds : %ld\n",
//     // 	current_time.tv_sec, current_time.tv_usec);
// 		usleep(9400);
// 		data->cur_time += 10;
// 		i = 0;
// 		temp = 0;
// 		while (i < data->num_of_philos)
// 		{
// 			if (data->philo[i].has_eaten == 1)
// 			{
// 				// printf("has_eaten!!\n");
// 				data->philo[i].has_eaten = false;
// 				data->philo[i].living = 0;
// 			}
// 			if(data->philo[i].times_eaten == data->num_of_meals)
// 			    temp++;
// 			if(temp == data->num_of_philos && data->num_of_meals > 0)
// 			{
// 			    printf("All mouth are feed!!!\n");
// 				all_feed = true;
// 			    break;
// 			}
// 			data->philo[i].living += 10;
// 			if (data->philo[i].living == data->time_to_die)
// 			{
// 				gettimeofday(&death_time, NULL);
// 				time_elapsed = ft_timestamp(&data->start_time, &death_time);
// 				printf("%ld %d died\n", time_elapsed, i + 1);
// 				printf("livied for %d died\n", data->philo[i].living);

// 				// make function to close app
// 				exit(0);
// 			}
// 			i++;
// 		}
// 		// ft_tester(data, 1);
// 		// printf("seconds passed: %d\n", data->cur_time);
// 		if (all_feed == true)
// 			break ;
// 	}
// }