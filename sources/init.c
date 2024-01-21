/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 09:19:23 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:26:11 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "philosophers.h"

void	init_settings(t_roundtable *table, char **argv)
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

static void	redirect_end_to_start(t_roundtable *table)
{
	t_philosopher	*tmp;

	tmp = table->philosophers;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = table->philosophers;
}

static void	add_philosopher(t_philosopher **head, t_philosopher *node)
{
	t_philosopher	*tmp;

	tmp = *head;
	if (!tmp)
	{
		*head = node;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = node;
}

static int	init_philosophers(t_roundtable *table)
{
	int				index;
	t_philosopher	*philosopher;

	index = 0;
	while (index++ < table->settings.philosopher_amount)
	{
		philosopher = malloc(sizeof(t_philosopher));
		if (!philosopher)
			return (printf("Error during malloc 'Philosophers %u'", index), 1);
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
	return (0);
}

int	init_table(t_roundtable *table, char **argv)
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
