#include "../include/philosophers.h"


void ft_print_state(data_t *data, long time_elapsed, int philo, char *msg)
{	
	pthread_mutex_lock(&data->print_mutex);
	printf("%ld %d %s\n", time_elapsed, philo + 1, msg);
	pthread_mutex_unlock(&data->print_mutex);
}

void	ft_eating(tread_data_t *tr_data, int left_fork, int right_fork)
{
	struct timeval start_eat, start_sleep;
	long time_elapsed;

	while (1)
	{
		if (tr_data->data->forks[left_fork] == 0 && tr_data->data->forks[right_fork] == 0)
		{
			tr_data->data->forks[left_fork] = 1;
			tr_data->data->forks[right_fork] = 1;
			tr_data->data->philo[tr_data->philo_index].cur_state = EAT;
			// tr_data->data->philo[tr_data->philo_index].has_eaten = true; // hmm to remove?
			tr_data->data->philo[tr_data->philo_index].times_eaten++;
			
			// printf("++philo nr %d sta/rt eating at %d minisec\n", tr_data->philo_index, tr_data->data->cur_time);
			
			// Capture start time
			gettimeofday(&start_eat, NULL);
			time_elapsed = ft_timestamp(&tr_data->data->start_time, &start_eat);
			tr_data->data->philo[tr_data->philo_index].last_meal = time_elapsed;
			ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index, "is eating");
			// long last_meal_time = tr_data->data->philo[tr_data->philo_index].last_meal; // last_meal should be in microseconds
			// long current_time = start_eat.tv_sec * 1000000 + start_eat.tv_usec; // current time in microseconds
			// time_elapsed = current_time - last_meal_time; // Calculate elapsed time since last meal
			// // printf("philo %d, time_elapsed %ld\n", tr_data->philo_index + 1, time_elapsed);
			// if(time_elapsed > 700000 && time_elapsed < 10000000)
			// 	exit(0);
			// Simulate eating
			usleep(tr_data->data->time_to_eat * TIME_UNIT);
			// gettimeofday(&start_eat, NULL);
			// time_elapsed = ft_timestamp(&tr_data->data->start_time, &start_eat);
			// tr_data->data->philo[tr_data->philo_index].last_meal = time_elapsed;
			// printf("%ld %d finish eating\n", time_elapsed, tr_data->philo_index + 1);
			// Calculate the time elapsed from last_meal to now
			// Update last_meal to current time
			// tr_data->data->philo[tr_data->philo_index].last_meal = current_time;

			// Release forks
			tr_data->data->forks[left_fork] = 0;
			tr_data->data->forks[right_fork] = 0;
			
			tr_data->data->philo[tr_data->philo_index].cur_state = SLEEP;
			gettimeofday(&start_sleep, NULL);
			time_elapsed = ft_timestamp(&tr_data->data->start_time, &start_sleep);
			ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index, "is sleeping");
			break;
		}
		// usleep(10); // adjust this
	}
}

bool ft_has_prioryty(tread_data_t *tr_data)
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
	struct timeval fork_taken, start_thinking;
	int				i;
	int				left_fork;
	int				right_fork;
	long time_elapsed;

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
			if(ft_has_prioryty(tr_data))
			{
				if (left_fork < right_fork)
				{	
					pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
					gettimeofday(&fork_taken, NULL);
					time_elapsed = ft_timestamp(&tr_data->data->start_time, &fork_taken);
						ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index , "has taken a fork");
					pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
					gettimeofday(&fork_taken, NULL);
					time_elapsed = ft_timestamp(&tr_data->data->start_time, &fork_taken);
						ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index, "has taken a fork");
				}
				else
				{
					pthread_mutex_lock(&tr_data->data->fork_mutexes[left_fork]);
					gettimeofday(&fork_taken, NULL);
					time_elapsed = ft_timestamp(&tr_data->data->start_time, &fork_taken);
						ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index , "has taken a fork");
					pthread_mutex_lock(&tr_data->data->fork_mutexes[right_fork]);
					gettimeofday(&fork_taken, NULL);
					time_elapsed = ft_timestamp(&tr_data->data->start_time, &fork_taken);
						ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index, "has taken a fork");
				}
				// usleep(10);
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
			gettimeofday(&start_thinking, NULL);
			time_elapsed = ft_timestamp(&tr_data->data->start_time, &start_thinking);
			ft_print_state(tr_data->data, time_elapsed, tr_data->philo_index, "is thinking");

		}
		if(tr_data->data->num_of_meals > 0) 
		{
			// printf("num of meals: %d\n", tr_data->data->philo[tr_data->philo_index].times_eaten);
			if(tr_data->data->philo[tr_data->philo_index].times_eaten == tr_data->data->num_of_meals)
			{
				printf("philo nr %d finished\n", tr_data->philo_index + 1); // remove this
				tr_data->data->philo[tr_data->philo_index].is_feed = true;
				break;
			}
		}
		usleep(100); // hmm adjust this
	}
}

void ft_monitor(data_t *data)
{
	struct timeval cur_time, death_time;
	int i;
	long time_elapsed;
	while(1)
	{
		usleep(10); // might be to big BUT only for big number for many philos
		gettimeofday(&cur_time, NULL);
		time_elapsed = ft_timestamp(&data->start_time, &cur_time);
		i = 0;
		while(i < data->num_of_philos)
		{
			long meal_times = time_elapsed - data->philo[i].last_meal;
			data->philo[i].living = meal_times;
			if(meal_times > data->time_to_die && !data->philo[i].is_feed)
			{
				gettimeofday(&death_time, NULL);
				time_elapsed = ft_timestamp(&data->start_time, &death_time);
				printf("%ld %d died\n", time_elapsed, i + 1);
				printf("lived for %ld died\n", meal_times);
				// close it in a nice way
				exit(0);
			}
			i++;
		}
		// ft_tester(data, 1);
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

	if(pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		perror("Failed to initialize mutex");
		exit(EXIT_FAILURE);
	}

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
	// ft_global_clock(data);
	ft_monitor(data);
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
	if(pthread_mutex_destroy(&data->print_mutex) != 0)
	{
		perror("Failed to destroy mutex");
		exit(EXIT_FAILURE);
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
	struct timeval start_time;

	setbuf(stdout, NULL); // Remove this !!!
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