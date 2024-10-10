#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#define EAT 0
#define SLEEP 1
#define THINK 2
#define TIME_UNIT 1000

typedef struct philo_s
{
    int philo_num;
    int eating; // ??
    int living;
    int sleeping; // ??
    int cur_state;
    int status_change; // ??
    int has_eaten;
    int times_eaten;
    bool is_feed;
    bool has_died;
    long last_meal;
} philo_t;

typedef struct data_s
{
    int num_of_philos;
    int num_of_meals;
    int time_to_eat;
    int time_to_die; 
    int time_to_sleep;
    int *forks;
    int *philo_states;
    int cur_time;
    bool philo_died;
    bool all_feed;
    struct timeval start_time;
    pthread_mutex_t *fork_mutexes;
    pthread_mutex_t print_mutex;
    pthread_mutex_t general;
    pthread_mutex_t death_mutex;
    philo_t *philo;
} data_t; 

typedef struct tread_data_s
{
    data_t *data;
    int philo_index;
} tread_data_t;


// libf utils

void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
int	    ft_atoi(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);

// utils

void	ft_tester(data_t *data, int key); // to remove
long ft_timestamp(struct timeval *start_time, struct timeval *cur_time);
void ft_print_state(data_t *data, long time_elapsed, int philo, char *msg);
bool ft_has_prioryty(tread_data_t *tr_data);


// actions

bool ft_death(data_t *data);

//

void ft_destroy_mutex(data_t *data);
void ft_close(data_t *data, pthread_t *th, tread_data_t **tr_data);
void ft_free_allocs(data_t *data, tread_data_t **tr_data);


#endif