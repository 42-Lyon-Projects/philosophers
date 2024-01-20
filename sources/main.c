/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbadaire <jbadaire@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 09:27:22 by jbadaire          #+#    #+#             */
/*   Updated: 2024/01/20 08:22:55 by jbadaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include "philosophers.h"

void free_philosophers(t_philosopher *philosophers)
{
	t_philosopher *current = philosophers;
	t_philosopher *next;
	int first_iteration = 1;

	while (current != philosophers || first_iteration)
	{
		next = current->next;
		free(current);
		current = next;
		first_iteration = 0;
	}
}

int main(int argc, char **argv)
{
	t_roundtable *table;

	table = malloc(sizeof(t_roundtable));
	if (!arguments_is_valids(argc, argv))
		return (free(table), 1);
	if (init_table(table, argv))
		return (destroy_mutexs(NULL, table), free(table), 1);
	if (create_threads(table))
		return (destroy_mutexs(NULL, table), free(table), 1);
	if (joining_threads(table))
		return (destroy_mutexs(NULL, table), free(table), 1);

	destroy_mutexs(NULL, table);
	free_philosophers(table->philosophers);
	free(table);
	return (0);
}
