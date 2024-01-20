/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:23 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 08:53:13 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "philosophers.h"

static int init_philosophers(t_roundtable *table);

void init_settings(t_roundtable *table, char **argv)
{
	table->settings.philosopher_amount = ft_atoi(argv[1]);
	table->settings.time_to_die = ft_atoi(argv[2]);
	table->settings.time_to_eat = ft_atoi(argv[3]);
	table->settings.time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->settings.max_meals = ft_atoi(argv[5]);
	else
		table->settings.max_meals = -1;
}

int init_table_mutex(t_roundtable *table)
{
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		return (printf("Error during mutex initialisation of 'Dead Mutex'"), 1);
	if (pthread_mutex_init(&table->meal_mutex, NULL))
		return (printf("Error during mutex initialisation of 'Meal Mutex'"), 1);
	if (pthread_mutex_init(&table->write_mutex, NULL))
		return (printf("Error during mutex initialisation of 'Write Mutex'"), 1);
	if (pthread_mutex_init(&table->end_mutex, NULL))
		return (printf("Error during mutex initialisation of 'End Mutex'"), 1);
	return (0);
}

static void redirect_end_to_start(t_roundtable *table)
{
	t_philosopher *tmp;

	tmp = table->philosophers;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = table->philosophers;
}

int init_table(t_roundtable *table, char **argv)
{
	table->has_dead = 0;
	init_settings(table, argv);
	init_table_mutex(table);
	table->philosophers = NULL;
	table->must_end = 0;
	if (init_philosophers(table))
		return (1);
	redirect_end_to_start(table);
	return (0);
}

void add_philosopher(t_philosopher **philosophers, t_philosopher *new_node)
{
	t_philosopher *tmp;

	tmp = *philosophers;
	if (!tmp)
	{
		*philosophers = new_node;
		return;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

int init_philosophers_mutexs(t_philosopher *philosopher)
{
	if (pthread_mutex_init(&philosopher->state_mutex, NULL))
		return (printf("Error during mutex initialisation of 'State Mutex %d'", philosopher->philosopher_id), 1);
	if (pthread_mutex_init(&philosopher->fork_mutex, NULL))
		return (printf("Error during mutex initialisation of 'Fork Mutex %d'", philosopher->philosopher_id), 1);
	if (pthread_mutex_init(&philosopher->meal_mutex, NULL))
		return (printf("Error during mutex initialisation of 'Philosopher Meal Mutex %d'",
			        philosopher->philosopher_id), 1);
	return (0);
}

static int init_philosophers(t_roundtable *table)
{
	int index;
	t_philosopher *philosopher;

	index = 0;
	while (index++ < table->settings.philosopher_amount)
	{
		philosopher = malloc(sizeof(t_philosopher));
		if (!philosopher)
			return (printf("Error during malloc of 'Philosophers %u'", index), 1);
		philosopher->philosopher_id = index;
		philosopher->state = THINKING;
		philosopher->fork_available = 1;
		philosopher->meals_amount = 0;
		philosopher->last_meal_timestamp = get_current_time_in_ms();
		philosopher->table = table;
		philosopher->next = NULL;
		if (init_philosophers_mutexs(philosopher))
			return (1);
		add_philosopher(&table->philosophers, philosopher);
	}
	table->start_timestamp = get_current_time_in_ms();
	return (0);
}

int create_threads(t_roundtable *table)
{
	int index;
	int error_code;
	pthread_t watchdog_thread;
	t_philosopher *tmp;

	error_code = pthread_create(&watchdog_thread, NULL, &watchdog_task, table->philosophers);
	if (error_code)
		return (destroy_mutexs("Error during creation of 'Watchdog' Thread.", table),\
		        error_code);
	table->watchdog_thread = watchdog_thread;
	tmp = table->philosophers;
	index = 0;
	while (index++ < table->settings.philosopher_amount)
	{
		error_code = pthread_create(&tmp->thread, NULL, &philosopher_task, tmp);
		tmp = tmp->next;
		if (error_code)
		{
			destroy_mutexs("Error during creation of 'Philosopher' Thread.", table);
			return (error_code);
		}
	}
	return (0);
}

int joining_threads(t_roundtable *table)
{
	size_t index;
	int error_code;
	t_philosopher *philosopher;

	error_code = pthread_join(table->watchdog_thread, NULL);
	if (error_code)
	{
		destroy_mutexs("Error attempted during pthread_join of 'Watchdog' Thread.", table);
		return (error_code);
	}
	index = 0;
	philosopher = table->philosophers;
	while (philosopher && index < table->settings.philosopher_amount)
	{
		error_code = pthread_join(philosopher->thread, NULL);
		if (error_code)
		{
			destroy_mutexs("Error attempted during pthread_join of 'Philosopher' Thread.", table);
			return (error_code);
		}
		philosopher = philosopher->next;
		index++;
	}
	return (0);
}

void destroy_mutexs(char *message, t_roundtable *table)
{
	t_philosopher *tmp;
	size_t index;

	tmp = table->philosophers;
	index = 0;
	if (message)
		printf("%s\n", message);
	pthread_mutex_destroy(&table->write_mutex);
	pthread_mutex_destroy(&table->meal_mutex);
	pthread_mutex_destroy(&table->dead_mutex);
	pthread_mutex_destroy(&table->end_mutex);
	while (index < table->settings.philosopher_amount)
	{
		pthread_mutex_destroy(&tmp->state_mutex);
		pthread_mutex_destroy(&tmp->fork_mutex);
		pthread_mutex_destroy(&tmp->meal_mutex);
		tmp = tmp->next;
		index++;
	}
}
