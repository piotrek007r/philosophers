#include "../include/philosophers.h"

void	ft_tester(data_t *data, int key)
{
	if (key == 1)
	{
		for (int i = 0; i < data->num_of_philos; i++)
		{
			printf("philo %d, current status: %d eated: %d living %d\n", i + 1, data->philo[i].cur_state,
				data->philo[i].times_eaten,
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

long ft_timepassed(struct timeval start, struct timeval end)
{
	long seconds_diff = end.tv_sec - start.tv_sec;
    long useconds_diff = end.tv_usec - start.tv_usec;

    return (seconds_diff * 1000000) + useconds_diff;
}

void	ft_eating(tread_data_t *tr_data, int left_fork, int right_fork)
{
	struct timeval start;
	long time_elapsed;

	while (1)
	{
		if (tr_data->data->forks[left_fork] == 0 && tr_data->data->forks[right_fork] == 0)
		{
			tr_data->data->forks[left_fork] = 1;
			tr_data->data->forks[right_fork] = 1;
			tr_data->data->philo[tr_data->philo_index].cur_state = EAT;
			tr_data->data->philo[tr_data->philo_index].has_eaten = true;
			tr_data->data->philo[tr_data->philo_index].times_eaten++;
			
			// printf("++philo nr %d sta/rt eating at %d minisec\n", tr_data->philo_index, tr_data->data->cur_time);
			
			// Capture start time
			gettimeofday(&start, NULL);
			long last_meal_time = tr_data->data->philo[tr_data->philo_index].last_meal; // last_meal should be in microseconds
			long current_time = start.tv_sec * 1000000 + start.tv_usec; // current time in microseconds
			time_elapsed = current_time - last_meal_time; // Calculate elapsed time since last meal
			printf("philo %d, time_elapsed %ld\n", tr_data->philo_index + 1, time_elapsed);
			if(time_elapsed > 700000 && time_elapsed < 10000000)
				exit(0);
			// Simulate eating
			usleep(tr_data->data->time_to_eat * TIME_UNIT);
			
			// Calculate the time elapsed from last_meal to now

			
			// Update last_meal to current time
			tr_data->data->philo[tr_data->philo_index].last_meal = current_time;

			// Release forks
			tr_data->data->forks[left_fork] = 0;
			tr_data->data->forks[right_fork] = 0;
			
			tr_data->data->philo[tr_data->philo_index].cur_state = SLEEP;
			break;
		}
		// usleep(10); // adjust this
	}
}

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

bool ft_relaxed_priority(tread_data_t *tr_data)
{
	int i;
	int cur;
	cur = tr_data->philo_index;
	i = 0;
	while(i < tr_data->data->num_of_philos)
	{
		if(tr_data->data->philo[i].living > tr_data->data->philo[cur].living)
			return false;
		i++;
	}
	return true;
}

void	*ft_philo_routine(void *arg)
{
	tread_data_t	*tr_data;
	int				i;
	int				left_fork;
	int				right_fork;

	tr_data = (tread_data_t *)arg;
	i = 0;
	right_fork = tr_data->philo_index;
	if (tr_data->philo_index == 0)
		left_fork = tr_data->data->num_of_philos - 1;
	else
		left_fork = tr_data->philo_index - 1;
	while (1)
	{
		if (tr_data->data->philo[tr_data->philo_index].cur_state == THINK)
		{
			if(ft_relaxed_priority(tr_data))
			{
			if (left_fork < right_fork)
			{
				pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
				pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
			}
			else
			{
				pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
				pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
			}
			ft_eating(tr_data, left_fork, right_fork);

			pthread_mutex_unlock(&tr_data->data->fork_mutexes[left_fork]);
			pthread_mutex_unlock(&tr_data->data->fork_mutexes[right_fork]);

			}
			// printf("unlocking fork: %d and %d\n", left_fork, right_fork);

		}
		if (tr_data->data->philo[tr_data->philo_index].cur_state == SLEEP)
		{
			// printf("++philo nr %d is taking a nap at %d minisec\n", tr_data->philo_index,
			// 	tr_data->data->cur_time);
			usleep(tr_data->data->time_to_sleep * TIME_UNIT); // Sleeping time
			// printf("++philo nr %d has awake at %d minisec\n", tr_data->philo_index,
			// 	tr_data->data->cur_time);
			tr_data->data->philo[tr_data->philo_index].cur_state = THINK;
		}
		if(tr_data->data->num_of_meals > 0) 
		{
			// printf("num of meals: %d\n", tr_data->data->philo[tr_data->philo_index].times_eaten);
			if(tr_data->data->philo[tr_data->philo_index].times_eaten == tr_data->data->num_of_meals)
			{
				printf("philo nr %d finished\n", tr_data->philo_index);
				break;
			}
		}
		// i++;
		// if(i > 0)
		//     break ;
		// usleep(100);
	}
}

void	ft_global_clock(data_t *data)
{
	int	i;
	int	temp;
	int	seconds;
	int all_feed;

	data->cur_time = 0;
	all_feed = false;
	while (1)
	{
  	// printf("seconds : %ld\nmicro seconds : %ld\n",
    // 	current_time.tv_sec, current_time.tv_usec);
		usleep(9600);
		i = 0;
		temp = 0;
		while (i < data->num_of_philos)
		{
			if (data->philo[i].has_eaten == 1)
			{
				// printf("has_eaten!!\n");
				data->philo[i].has_eaten = false;
				data->philo[i].living = 0;
			}
			if(data->philo[i].times_eaten == data->num_of_meals)
			    temp++;
			if(temp == data->num_of_philos && data->num_of_meals > 0)
			{
			    printf("All mouth are feed!!!\n");
				all_feed = true;
			    break;
			}
			data->philo[i].living += 10;
			if (data->philo[i].living == data->time_to_die)
			{
				printf("Die motherfucker!\n");
				// make function to close app
				exit(0);
			}
			i++;
		}
		ft_tester(data, 1);
		data->cur_time += 10;
		// printf("seconds passed: %d\n", data->cur_time);
		if (all_feed == true)
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
		// usleep(100); consider this
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
		data->philo[i].cur_state = THINK;
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