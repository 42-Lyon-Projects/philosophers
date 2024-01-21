/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 11:32:09 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:37:19 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "philosophers.h"

/**
 * @brief Gets the current time in milliseconds using gettimeofday.
 *
 * @brief This func retrieves the current time in ms.
 * in timeval time.tv_sec represents the number of seconds.
 * in timeval time.tv_usec represents the number of microseconds.
 * So time.tv_sec * 1000 converts seconds to milliseconds and
 * time.tv_usec / 1000 converts microseconds to milliseconds.
 *
 * Adding these two together gives the total time in milliseconds.
 * So, the expression time.tv_sec * 1000 + time.tv_usec / 1000
 * calculates the total time in milliseconds, combining seconds and microseconds.
 * @return The current time in milliseconds.
 */
useconds_t	get_current_time_in_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("An error occured during getting time of the Day !\n");
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

/**
 * @brief Sleeps for the specified duration in milliseconds.
 *
 * This function sleeps for the given duration in milliseconds using
 * usleep with a granularity of 500 microseconds.
 *
 * @param ms The duration to sleep in milliseconds.
 * @return 0 on success.
 */
int	sleep_ms(useconds_t ms, t_roundtable *table)
{
	useconds_t	at_sleep;

	at_sleep = get_current_time_in_ms();
	while ((get_current_time_in_ms() - at_sleep) < ms && !must_stop(table))
		usleep(500);
	return (0);
}

useconds_t	time_elapsed_after_start(useconds_t start_timestamp)
{
	return (get_current_time_in_ms() - start_timestamp);
}
