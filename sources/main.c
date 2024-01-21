/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 09:27:22 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/21 13:28:18 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "philosophers.h"

void	free_philosophers(t_philosopher *philosophers)
{
	t_philosopher	*current;
	t_philosopher	*next;
	int				first_iteration;

	current = philosophers;
	first_iteration = 1;
	while (current != philosophers || first_iteration)
	{
		next = current->next;
		if (current)
			free(current);
		current = next;
		first_iteration = 0;
	}
}

int	main(int argc, char **argv)
{
	t_roundtable	*table;

	table = malloc(sizeof(t_roundtable));
	if (!arguments_is_valids(argc, argv))
		return (free(table), 1);
	if (init_table(table, argv))
		return (free_philosophers(table->philosophers), \
			destroy_mutexs(table), free(table), 1);
	if (create_threads(table))
		return (free_philosophers(table->philosophers), \
			destroy_mutexs(table), free(table), 1);
	if (joining_threads(table))
		return (free_philosophers(table->philosophers), \
			destroy_mutexs(table), free(table), 1);
	destroy_mutexs(table);
	free_philosophers(table->philosophers);
	free(table);
	return (0);
}
