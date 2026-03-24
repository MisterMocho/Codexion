/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:47:05 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 20:19:47 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_hub *hub)
{
	int	i;

	hub->dongles = malloc(sizeof(t_dongle *) * hub->num_coders);
	if (!hub->dongles)
		return (1);
	i = 0;
	while (i < hub->num_coders)
	{
		hub->dongles[i] = malloc(sizeof(t_dongle));
		if (!hub->dongles[i])
			return (1);
		if (pthread_mutex_init(&hub->dongles[i]->mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&hub->dongles[i]->cond, NULL) != 0)
			return (1);
		hub->dongles[i]->queue = malloc(sizeof(t_coder *) * hub->num_coders);
		if (!hub->dongles[i]->queue)
			return (1);
		hub->dongles[i]->queue_size = 0;
		hub->dongles[i]->is_held = 0;
		hub->dongles[i]->available_at = 0;
		i++;
	}
	return (0);
}

static void	link_coders(t_hub *hub)
{
	int	i;
	int	prev_idx;
	int	next_idx;

	i = 0;
	while (i < hub->num_coders)
	{
		prev_idx = (i - 1 + hub->num_coders) % hub->num_coders;
		next_idx = (i + 1) % hub->num_coders;
		hub->coders[i]->prev = hub->coders[prev_idx];
		hub->coders[i]->next = hub->coders[next_idx];
		i++;
	}
}

static int	init_coders(t_hub *hub)
{
	int	i;

	hub->coders = malloc(sizeof(t_coder *) * hub->num_coders);
	if (!hub->coders)
		return (1);
	i = 0;
	while (i < hub->num_coders)
	{
		hub->coders[i] = malloc(sizeof(t_coder));
		if (!hub->coders[i])
			return (1);
		if (pthread_mutex_init(&hub->coders[i]->coder_mutex, NULL) != 0)
			return (1);
		hub->coders[i]->id = i + 1;
		hub->coders[i]->compiles_done = 0;
		hub->coders[i]->hub = hub;
		hub->coders[i]->left_dongle = hub->dongles[i];
		hub->coders[i]->right_dongle = hub->dongles[(i + 1) % hub->num_coders];
		i++;
	}
	link_coders(hub);
	return (0);
}

int	init_simulation(t_hub *hub)
{
	if (pthread_mutex_init(&hub->print_mutex, NULL) != 0)
		return (1);
	if (init_dongles(hub) != 0)
		return (1);
	if (init_coders(hub) != 0)
		return (1);
	return (0);
}
