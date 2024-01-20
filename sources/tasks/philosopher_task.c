/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_task.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 17:56:51 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 09:01:37 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

#include "philosophers.h"

int is_dead(t_philosopher *philosopher)
{
	int has_dead;

	if (must_exit(philosopher->table))
		return (0);
	pthread_mutex_lock(&philosopher->table->dead_mutex);
	has_dead = philosopher->table->has_dead;
	pthread_mutex_unlock(&philosopher->table->dead_mutex);
	return has_dead;
}

void philosopher_think(t_philosopher *philo)
{
	print_message("is thinking", philo);
}

void philosopher_change_state(t_philosopher *philosopher, const t_state state)
{
	if (must_exit(philosopher->table))
		return;
	pthread_mutex_lock(&philosopher->state_mutex);
	philosopher->state = state;
	pthread_mutex_unlock(&philosopher->state_mutex);
}

void philosopher_sleep(t_philosopher *philosopher)
{
	philosopher_change_state(philosopher, SLEEPING);
	print_message("is sleeping", philosopher);
	sleep_ms(philosopher->table->settings.time_to_sleep);
}

void take_fork(t_philosopher *philosopher, t_philosopher *locked)
{
	if (must_exit(philosopher->table))
		return;
	pthread_mutex_lock(&locked->fork_mutex);
	if (locked->fork_available)
	{
		locked->fork_available = 0;
		print_message("has taken a fork", philosopher);
	}
	else
		print_message("Can't take a fork", locked);
}

void drop_fork(t_philosopher *philosopher)
{
	philosopher->next->fork_available = 1;
	pthread_mutex_unlock(&philosopher->fork_mutex);
}

void philosopher_increment_meal(t_philosopher *philosopher)
{
	if (must_exit(philosopher->table))
		return;
	pthread_mutex_lock(&philosopher->meal_mutex);
	philosopher->last_meal_timestamp = get_current_time_in_ms();
	philosopher->meals_amount++;
	pthread_mutex_unlock(&philosopher->meal_mutex);
}

void philosopher_eat(t_philosopher *philosopher)
{
	if (must_exit(philosopher->table))
		return;
	take_fork(philosopher, philosopher);
	if (philosopher->table->settings.philosopher_amount == 1)
	{
		sleep_ms(philosopher->table->settings.time_to_die + 1);
		drop_fork(philosopher);
		return;
	}
	take_fork(philosopher, philosopher->next);
	philosopher_change_state(philosopher, EATING);
	print_message("is eating", philosopher);
	drop_fork(philosopher);
	drop_fork(philosopher->next);
	philosopher_increment_meal(philosopher);
	sleep_ms(philosopher->table->settings.time_to_sleep);
}

void *philosopher_task(void *pointer)
{
	t_philosopher *philo;

	philo = (t_philosopher *) pointer;
	if (must_exit(philo->table))
		return (pointer);
	if ((philo->philosopher_id % 2) == 0)
	{
		philosopher_think(philo);
		sleep_ms(philo->table->settings.time_to_sleep);
	}
	while (!is_dead(philo))
	{
		if (must_exit(philo->table))
			return (pointer);
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (pointer);
}
