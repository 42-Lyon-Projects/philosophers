/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 19:43:17 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:53:55 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <unistd.h>

# define MAX_PHILOSOPHERS 200

typedef enum e_state
{
	THINKING,
	EATING,
	SLEEPING
}	t_state;

typedef struct s_philosopher
{
	int						philosopher_id;
	pthread_t				thread;

	pthread_mutex_t			state_mutex;
	t_state					state;

	pthread_mutex_t			meal_mutex;
	useconds_t				last_meal_timestamp;
	int						meals_amount;

	int						fork_available;
	pthread_mutex_t			fork_mutex;

	struct s_philosopher	*next;
	struct s_roundtable		*table;
}	t_philosopher;

typedef struct s_settings
{
	int			philosopher_amount;
	useconds_t	time_to_die;
	useconds_t	time_to_eat;
	useconds_t	time_to_sleep;
	int			max_meals;
}	t_settings;

typedef struct s_roundtable
{
	int				has_dead;
	useconds_t		start_timestamp;
	t_settings		settings;

	pthread_mutex_t	write_mutex;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	meal_mutex;

	int				must_end;
	pthread_mutex_t	end_mutex;

	t_philosopher	*philosophers;
	pthread_t		watchdog_thread;
}	t_roundtable;

size_t		ft_atoi(const char *str);
void		print_message(const char *message, t_philosopher *philosopher);
void		unsecure_print(const char *message, t_philosopher *philosopher);

int			arguments_is_valids(int argc, char **argv);
int			init_table(t_roundtable *philo, char **argv);
int			init_philosophers_mutexs(t_philosopher *philosopher);
int			init_table_mutex(t_roundtable *table);
int			create_threads(t_roundtable *table);
int			joining_threads(t_roundtable *table);

void		philosopher_change_state(t_philosopher *philosopher, \
	const t_state state);
int			philosopher_think(t_philosopher *philosopher);
void		philosopher_sleep(t_philosopher *philosopher);
int			philosopher_eat(t_philosopher *philosopher);

void		drop_fork(t_philosopher *philosopher);
void		wait_fork(t_philosopher *philosopher);
int			take_fork(t_philosopher *philosopher, t_philosopher *locked);
void		philosopher_increment_meal(t_philosopher *philosopher);

void		*philosopher_task(void *ptr);
int			sleep_ms(useconds_t ms, t_roundtable *table);

int			must_exit(t_roundtable *table);
int			has_dead(t_roundtable *table);
int			must_stop(t_roundtable *table);
int			philosopher_must_dead(t_philosopher *philosopher, \
	size_t time_to_die);
void		*watchdog_task(void *ptr);

void		destroy_mutexs(t_roundtable *table);

useconds_t	get_current_time_in_ms(void);
useconds_t	time_elapsed_after_start(useconds_t start_timestamp);

#endif