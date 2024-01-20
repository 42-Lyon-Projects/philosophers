/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 00:13:59 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 02:41:18 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "philosophers.h"

static int ft_is_number(const char c)
{
	return (c >= '0' && c <= '9');
}

static int ft_is_numbers(const char *str)
{
	size_t index;

	index = 0;
	while (str[index])
	{
		if (!ft_is_number(str[index]))
			return (0);
		index++;
	}
	return (1);
}

size_t ft_atoi(const char *str)
{
	size_t	index;
	size_t	result;
	int		is_pos;

	index = 0;
	is_pos = 1;
	result = 0;
	while (ft_is_number(str[index]))
	{
		if (result != ((result * 10 + (is_pos * (str[index] - '0'))) / 10))
			return (((is_pos + 1) / 2 / -1));
		result = result * 10 + (str[index++] - 48);
	}
	return (result);
}

/**
 * @brief Validates the CLI arguments for the philosopher program.
 *
 * This function checks if the CLI arguments are valid for the philosopher program.
 * It verifies the correct number of arguments, the validity of numeric values, and their ranges.
 *
 * @param argc The number of CLI arguments.
 * @param argv The array of CLI argument strings.
 * @return 1 if the arguments are valid, 0 if it invalid.
 */
int arguments_is_valids(const int argc, char **argv)
{
	size_t	philosophers_number;

	if (argc != 5 && argc != 6)
		return (printf("Error: wrong number of arguments\n"), 0);
	philosophers_number = ft_atoi(argv[1]);
	if (!ft_is_numbers(argv[1]) || philosophers_number < 1 || philosophers_number > MAX_PHILOSOPHERS)
		return (printf("Philosophers Numbers must be positive number in range 1 - %d", MAX_PHILOSOPHERS), 0);
	if (!ft_is_numbers(argv[2]) || ft_atoi(argv[2]) < 1)
		return (printf("Philosophers time to die must be a positive Number superior of 0 (ms) !"), 0);
	if (!ft_is_numbers(argv[3]) || ft_atoi(argv[3]) < 1)
		return (printf("Philosophers time to eat must be a positive Number superior of 0 (ms) !"), 0);
	if (!ft_is_numbers(argv[4]) || ft_atoi(argv[4]) < 1)
		return (printf("Philosophers time to sleep must be a positive Number superior of 0 (ms)"), 0);
	if (argc == 6 && (!ft_is_numbers(argv[4]) || ft_atoi(argv[4]) < 1))
		return (printf("Philosophers number of meals must be a positive Number superior of 0"), 0);
	return (1);
}