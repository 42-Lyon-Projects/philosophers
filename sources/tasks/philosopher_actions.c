/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 12:58:14 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:49:16 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

void	philosopher_change_state(t_philosopher *philosopher, \
	const t_state state)
{
	pthread_mutex_lock(&philosopher->state_mutex);
	philosopher->state = state;
	pthread_mutex_unlock(&philosopher->state_mutex);
}

int	philosopher_think(t_philosopher *philosopher)
{
	if (must_stop(philosopher->table))
		return (1);
	print_message("is thinking", philosopher);
	return (0);
}

void	philosopher_sleep(t_philosopher *philosopher)
{
	if (must_stop(philosopher->table))
		return ;
	philosopher_change_state(philosopher, SLEEPING);
	print_message("is sleeping", philosopher);
	sleep_ms(philosopher->table->settings.time_to_sleep, philosopher->table);
}

int	philosopher_eat(t_philosopher *philosopher)
{
	if (!take_fork(philosopher, philosopher))
		return (1);
	philosopher_change_state(philosopher, EATING);
	print_message("is eating", philosopher);
	philosopher_increment_meal(philosopher);
	sleep_ms(philosopher->table->settings.time_to_eat, philosopher->table);
	drop_fork(philosopher->next);
	drop_fork(philosopher);
	return (0);
}
