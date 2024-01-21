/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watchdog_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 13:07:03 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:07:03 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	must_exit(t_roundtable *table)
{
	pthread_mutex_lock(&table->end_mutex);
	if (table->must_end)
	{
		return (pthread_mutex_unlock(&table->end_mutex), 1);
	}
	return (pthread_mutex_unlock(&table->end_mutex), 0);
}

int	has_dead(t_roundtable *table)
{
	pthread_mutex_lock(&table->dead_mutex);
	if (table->has_dead)
	{
		return (pthread_mutex_unlock(&table->dead_mutex), 1);
	}
	return (pthread_mutex_unlock(&table->dead_mutex), 0);
}

int	must_stop(t_roundtable *table)
{
	return (must_exit(table) || has_dead(table));
}

int	philosopher_must_dead(t_philosopher *philosopher, size_t time_to_die)
{
	pthread_mutex_lock(&philosopher->meal_mutex);
	pthread_mutex_lock(&philosopher->state_mutex);
	if ((get_current_time_in_ms() - philosopher->last_meal_timestamp) \
		>= time_to_die && philosopher->state != EATING)
	{
		pthread_mutex_unlock(&philosopher->state_mutex);
		pthread_mutex_unlock(&philosopher->meal_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosopher->state_mutex);
	pthread_mutex_unlock(&philosopher->meal_mutex);
	return (0);
}
