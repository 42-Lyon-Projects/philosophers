/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:12:16 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 12:17:00 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include "philosophers.h"

void print_message(const char *message, t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->table->write_mutex);
	if (must_stop(philosopher->table) == 0)
	{
		printf("%u %u %s\n", time_elapsed_after_start(philosopher->table->start_timestamp),\
			philosopher->philosopher_id, message);
	}
	pthread_mutex_unlock(&philosopher->table->write_mutex);
}
