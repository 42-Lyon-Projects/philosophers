/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:12:16 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 06:20:46 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>

#include "philosophers.h"

void	print_message(const char *message, t_philosopher *philosopher)
{
	if (!is_dead(philosopher))
	{
		if (must_exit(philosopher->table))
			return;
		pthread_mutex_lock(&philosopher->table->write_mutex);
		printf("%u %u %s\n", time_elapsed_after_start(philosopher->table->start_timestamp),\
			philosopher->philosopher_id, message);
		pthread_mutex_unlock(&philosopher->table->write_mutex);
	}
}
