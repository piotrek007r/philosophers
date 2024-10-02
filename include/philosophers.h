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


// libf utils

void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
int	    ft_atoi(const char *str);
void	*ft_calloc(size_t nmemb, size_t size);


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
    pthread_mutex_t *fork_mutexes;
    philo_t *philo;
} data_t; 

typedef struct tread_data_s
{
    data_t *data;
    int philo_index;
} tread_data_t;
 
#endif