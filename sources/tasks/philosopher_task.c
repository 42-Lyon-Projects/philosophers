/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_task.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 17:56:51 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 16:33:42 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

void	drop_fork(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->fork_mutex);
	philosopher->fork_available = 1;
	pthread_mutex_unlock(&philosopher->fork_mutex);
}

void	wait_fork(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->fork_mutex);
	while (!philosopher->fork_available)
	{
		pthread_mutex_unlock(&philosopher->fork_mutex);
		usleep(1);
		pthread_mutex_lock(&philosopher->fork_mutex);
	}
	philosopher->fork_available = 0;
	pthread_mutex_unlock(&philosopher->fork_mutex);
}

int	take_fork(t_philosopher *philosopher, t_philosopher *locked)
{
	if (must_stop(philosopher->table))
		return (0);
	wait_fork(locked->next);
	if (must_stop(philosopher->table))
		return (drop_fork(locked->next), 0);
	print_message("has taken a fork", philosopher);
	if (philosopher->table->settings.philosopher_amount <= 1)
		return (drop_fork(locked->next), 0);
	wait_fork(locked);
	if (must_stop(philosopher->table))
		return (drop_fork(locked), drop_fork(locked->next), 0);
	print_message("has taken a fork", philosopher);
	return (1);
}

void	philosopher_increment_meal(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->meal_mutex);
	philosopher->last_meal_timestamp = get_current_time_in_ms();
	philosopher->meals_amount++;
	pthread_mutex_unlock(&philosopher->meal_mutex);
}

void	*philosopher_task(void *pointer)
{
	t_philosopher	*philo;

	philo = (t_philosopher *) pointer;
	if ((philo->philosopher_id % 2))
	{
		if (philosopher_think(philo))
			return (NULL);
		sleep_ms(philo->table->settings.time_to_eat, philo->table);
	}
	while (!must_stop(philo->table))
	{
		if (philosopher_eat(philo))
			return (pointer);
		philosopher_sleep(philo);
		if (philosopher_think(philo))
			return (pointer);
	}
	return (pointer);
}
