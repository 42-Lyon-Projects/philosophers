/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog_task.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:35:18 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 08:27:42 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "philosophers.h"

int must_exit(t_roundtable *table)
{
	int must_exit;

	pthread_mutex_lock(&table->end_mutex);
	must_exit = table->must_end;
	pthread_mutex_unlock(&table->end_mutex);
	return (must_exit);
}

int	philosopher_must_dead(t_philosopher *philosopher, size_t time_to_die)
{
	pthread_mutex_lock(&philosopher->meal_mutex);
	pthread_mutex_lock(&philosopher->state_mutex);
	if ((get_current_time_in_ms() - philosopher->last_meal_timestamp) >= time_to_die && philosopher->state != EATING)
	{
		pthread_mutex_unlock(&philosopher->state_mutex);
		pthread_mutex_unlock(&philosopher->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosopher->state_mutex);
	pthread_mutex_unlock(&philosopher->meal_mutex);
	return (0);
}

int	kill_philosopher(t_philosopher *philosophers)
{
	int	index;
	t_philosopher *tmp;

	index = 0;
	tmp = philosophers;
	while (index++ < philosophers->table->settings.philosopher_amount)
	{
		if (philosopher_must_dead(tmp, philosophers->table->settings.time_to_die))
		{
			print_message("died", tmp);
			pthread_mutex_lock(&tmp->table->dead_mutex);
			philosophers->table->has_dead = 1;
			pthread_mutex_unlock(&tmp->table->dead_mutex);
			return (1);
		}
		tmp = tmp->next;
		index++;
	}
	return (0);
}

int philosopher_has_max_meals(t_philosopher *philosopher)
{
	int	has_max_meals;
	pthread_mutex_lock(&philosopher->meal_mutex);
	has_max_meals = philosopher->meals_amount >= philosopher->table->settings.max_meals;
	pthread_mutex_unlock(&philosopher->meal_mutex);
	return (has_max_meals);
}

int	philosophers_end_meals(t_roundtable *table)
{
	int	index;
	int	all_philosphers_have_max_meals;
	t_settings	settings;
	t_philosopher *tmp;

	index = 0;
	all_philosphers_have_max_meals = 1;
	settings = table->settings;
	tmp = table->philosophers;
	if (settings.max_meals == -1)
		return (0);
	while (index++ < settings.philosopher_amount)
	{
		if(!philosopher_has_max_meals(tmp))
			all_philosphers_have_max_meals = 0;
		tmp = tmp->next;
	}
	if (all_philosphers_have_max_meals && !table->must_end)
	{
		pthread_mutex_lock(&table->end_mutex);
		table->must_end = 1;
		pthread_mutex_unlock(&table->end_mutex);
		return (1);
	}
	return (0);
}

void	*watchdog_task(void *ptr)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)ptr;
	while (1)
	{
		if (must_exit(philosopher->table))
			break ;
		//if (kill_philosopher(philosopher))
		//	break ;
	}
	return (ptr);
}

