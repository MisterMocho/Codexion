/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 21:22:36 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 20:31:19 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	clean_coders(t_hub *hub)
{
	int	i;

	if (!hub->coders)
		return ;
	i = 0;
	while (i < hub->num_coders)
	{
		if (hub->coders[i])
		{
			pthread_mutex_destroy(&hub->coders[i]->coder_mutex);
			free(hub->coders[i]);
		}
		i++;
	}
	free(hub->coders);
}

static void	clean_dongles(t_hub *hub)
{
	int	i;

	if (!hub->dongles)
		return ;
	i = 0;
	while (i < hub->num_coders)
	{
		if (hub->dongles[i])
		{
			pthread_mutex_destroy(&hub->dongles[i]->mutex);
			pthread_cond_destroy(&hub->dongles[i]->cond);
			if (hub->dongles[i]->queue)
				free(hub->dongles[i]->queue);
			free(hub->dongles[i]);
		}
		i++;
	}
	free(hub->dongles);
}

void	clean_simulation(t_hub *hub)
{
	clean_coders(hub);
	clean_dongles(hub);
	pthread_mutex_destroy(&hub->print_mutex);
}
