/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_task.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 17:56:51 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 12:28:35 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "philosophers.h"

void philosopher_think(t_philosopher *philosopher)
{
	print_message("is thinking", philosopher);
}

void philosopher_change_state(t_philosopher *philosopher, const t_state state)
{
	pthread_mutex_lock(&philosopher->state_mutex);
	philosopher->state = state;
	pthread_mutex_unlock(&philosopher->state_mutex);
}

void drop_fork(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->fork_mutex);
	philosopher->next->fork_available = 1;
	pthread_mutex_unlock(&philosopher->fork_mutex);
}

void philosopher_sleep(t_philosopher *philosopher)
{
	philosopher_change_state(philosopher, SLEEPING);
	print_message("is sleeping", philosopher);
	sleep_ms(philosopher->table->settings.time_to_sleep);
}

static void wait_fork(t_philosopher *philosopher)
{
	int fork_available;

	fork_available = 0;
	while (!fork_available)
	{
		pthread_mutex_lock(&philosopher->fork_mutex);
		fork_available = philosopher->fork_available;
		if (fork_available)
			philosopher->fork_available = 0;
		pthread_mutex_unlock(&philosopher->fork_mutex);
		usleep(1);
	}
}

int take_fork(t_philosopher *philosopher, t_philosopher *locked)
{
	wait_fork(locked->next);
	if (must_stop(philosopher->table))
		return (drop_fork(locked), 0);
	print_message("has taken a fork", philosopher);
	if (philosopher->table->settings.philosopher_amount <= 1)
		return (drop_fork(philosopher), 0);
	wait_fork(locked);
	if (must_stop(philosopher->table))
		return (drop_fork(locked), drop_fork(locked->next), 0);
	print_message("has taken a fork", philosopher);
	return (1);
}

void philosopher_increment_meal(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->meal_mutex);
	philosopher->last_meal_timestamp = get_current_time_in_ms();
	philosopher->meals_amount++;
	pthread_mutex_unlock(&philosopher->meal_mutex);
}

int philosopher_eat(t_philosopher *philosopher)
{
	if (!take_fork(philosopher, philosopher))
		return (1);
	philosopher_change_state(philosopher, EATING);
	print_message("is eating", philosopher);
	philosopher_increment_meal(philosopher);
	sleep_ms(philosopher->table->settings.time_to_eat);
	drop_fork(philosopher->next);
	drop_fork(philosopher);
	return (0);
}

void *philosopher_task(void *pointer)
{
	t_philosopher *philo;

	philo = (t_philosopher *) pointer;
	if ((philo->philosopher_id % 2) == 0)
	{
		philosopher_think(philo);
		sleep_ms(philo->table->settings.time_to_eat);
	}
	while (!must_stop(philo->table))
	{
		if(philosopher_eat(philo))
			return (pointer);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (pointer);
}
