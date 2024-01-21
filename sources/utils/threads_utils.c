/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 13:12:39 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:32:58 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

#include "philosophers.h"

int	create_threads(t_roundtable *table)
{
	int				index;
	pthread_t		watchdog_thread;
	t_philosopher	*tmp;

	table->start_timestamp = get_current_time_in_ms();
	if (pthread_create(&watchdog_thread, NULL, &watchdog_task, \
		table->philosophers))
	{
		destroy_mutexs(table);
		return (printf("Error during creation of 'Watchdog' Thread."), 0);
	}
	table->watchdog_thread = watchdog_thread;
	tmp = table->philosophers;
	index = 0;
	while (index++ < table->settings.philosopher_amount)
	{
		if (pthread_create(&tmp->thread, NULL, &philosopher_task, tmp))
		{
			printf("Error during creation of 'Philosopher' Thread.");
			destroy_mutexs(table);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	joining_threads(t_roundtable *table)
{
	int				index;
	t_philosopher	*philosopher;

	if (pthread_join(table->watchdog_thread, NULL))
	{
		printf("Error attempted during pthread_join of 'Watchdog' Thread.");
		destroy_mutexs(table);
		return (1);
	}
	index = 0;
	philosopher = table->philosophers;
	while (philosopher && index < table->settings.philosopher_amount)
	{
		if (pthread_join(philosopher->thread, NULL))
		{
			printf("Error attempted during join of 'Philosopher' Thread.");
			destroy_mutexs(table);
			return (1);
		}
		philosopher = philosopher->next;
		index++;
	}
	return (0);
}

int	init_philosophers_mutexs(t_philosopher *philosopher)
{
	if (pthread_mutex_init(&philosopher->state_mutex, NULL))
		return (printf("Error during initialisation of 'State Mutex %d'", \
			philosopher->philosopher_id), 1);
	if (pthread_mutex_init(&philosopher->fork_mutex, NULL))
		return (printf("Error during initialisation of 'Fork Mutex %d'", \
			philosopher->philosopher_id), 1);
	if (pthread_mutex_init(&philosopher->meal_mutex, NULL))
		return (printf("Error during initialisation of 'Philo Meal Mutex %d'", \
			philosopher->philosopher_id), 1);
	return (0);
}

int	init_table_mutex(t_roundtable *table)
{
	if (pthread_mutex_init(&table->dead_mutex, NULL))
		return (printf("Error during initialisation of 'Dead Mutex'"), 1);
	if (pthread_mutex_init(&table->meal_mutex, NULL))
		return (printf("Error during initialisation of 'Meal Mutex'"), 1);
	if (pthread_mutex_init(&table->write_mutex, NULL))
		return (printf("Error during initialisation of 'Write Mutex'"), 1);
	if (pthread_mutex_init(&table->end_mutex, NULL))
		return (printf("Error during initialisation of 'End Mutex'"), 1);
	return (0);
}

void	destroy_mutexs(t_roundtable *table)
{
	t_philosopher	*tmp;
	int				index;

	tmp = table->philosophers;
	index = 0;
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
