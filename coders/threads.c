/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:05:39 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 19:37:47 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wait_for_coders(t_hub *hub)
{
	int	i;

	i = 0;
	while (i < hub->num_coders)
	{
		pthread_join(hub->coders[i]->thread_id, NULL);
		i++;
	}
}

static int	create_coders(t_hub *hub, pthread_t *monitor)
{
	int			i;

	hub->start_time = get_time_in_ms();
	i = 0;
	while (i < hub->num_coders)
	{
		hub->coders[i]->last_compile_start = hub->start_time;
		if (pthread_create(&hub->coders[i]->thread_id, NULL,
				&coder_routine, hub->coders[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(monitor, NULL, &monitor_routine, hub) != 0)
		return (1);
	return (0);
}

int	start_simulation(t_hub *hub)
{
	pthread_t	monitor;

	if (create_coders(hub, &monitor) != 0)
		return (1);
	wait_for_coders(hub);
	pthread_join(monitor, NULL);
	return (0);
}
