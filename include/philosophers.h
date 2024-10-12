#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define EAT 0
# define SLEEP 1
# define THINK 2
# define TIME_UNIT 1000

typedef struct s_philo
{
	int				philo_num;
	int				living;
	int				cur_state;
	int				has_eaten;
	int				times_eaten;
	int				left_fork;
	int				right_fork;
	long			last_meal;
	bool			is_feed;
	bool			has_died;
}					t_philo;

typedef struct s_data
{
	int				num_of_philos;
	int				num_of_meals;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				*forks;
	int				*philo_states;
	int				cur_time;
	bool			philo_died;
	bool			all_feed;
	struct timeval	start_time;
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	general;
	pthread_mutex_t	death_mutex;
	t_philo			*philo;
}					t_data;

typedef struct t_tread_data
{
	t_data			*data;
	int				philo_index;
}					t_tread_data;

// utils

int					ft_atoi(const char *str);
void				*ft_calloc(size_t nmemb, size_t size);
long				ft_timestamp(struct timeval *start_time,
						struct timeval *cur_time);
void				ft_print_state2(t_data *data, long time_elapsed, int philo,
						char *msg);
bool				ft_has_prioryty(t_tread_data *tr_data);
bool				ft_death(t_tread_data *tr_data);
void				ft_print_state(t_tread_data *tr_data, char *str);
bool				ft_all_feed(t_tread_data *tr_data);

// actions

void				ft_eating(t_tread_data *tr_data, int left_fork,
						int right_fork);
void				ft_sleeping(t_tread_data *tr_data);

// close

void				ft_destroy_mutex(t_data *data);
void				ft_close(t_data *data, pthread_t *th,
						t_tread_data **tr_data);
void				ft_free_allocs(t_data *data, t_tread_data **tr_data);

// monitor

void				ft_monitor(t_data *data);
long				ft_monitor_iteration(t_data *data);
void				ft_monitor_check_death(t_data *data, long meal_times,
						long time_elapsed, int i);
long				ft_monitor_check_meals(t_data *data, long time_elapsed,
						int i);

// routine

void				ft_fork_indexing(t_tread_data *tr_data);
void				ft_fork_acquisition(t_tread_data *tr_data, int left_fork,
						int right_fork);
void				*ft_philo_routine(void *arg);

#endif