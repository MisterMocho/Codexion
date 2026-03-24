/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:38:36 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 20:44:25 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_coder *coder)
{
	long long	now;
	long long	last_start;

	now = get_time_in_ms();
	pthread_mutex_lock(&coder->coder_mutex);
	last_start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->coder_mutex);
	if ((now - last_start) >= coder->hub->time_to_burnout)
	{
		pthread_mutex_lock(&coder->hub->print_mutex);
		if (coder->hub->simulation_running)
		{
			printf("%lld %d burned out\n",
				now - coder->hub->start_time, coder->id);
			coder->hub->simulation_running = 0;
		}
		pthread_mutex_unlock(&coder->hub->print_mutex);
		return (1);
	}
	return (0);
}

static int	check_all_compiles(t_hub *hub)
{
	int	i;
	int	finished;

	if (hub->required_compiles <= 0)
		return (0);
	i = 0;
	finished = 0;
	while (i < hub->num_coders)
	{
		pthread_mutex_lock(&hub->coders[i]->coder_mutex);
		if (hub->coders[i]->compiles_done >= hub->required_compiles)
			finished++;
		pthread_mutex_unlock(&hub->coders[i]->coder_mutex);
		i++;
	}
	if (finished == hub->num_coders)
	{
		pthread_mutex_lock(&hub->print_mutex);
		hub->simulation_running = 0;
		pthread_mutex_unlock(&hub->print_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;
	int		i;

	hub = (t_hub *)arg;
	while (hub->simulation_running)
	{
		i = 0;
		while (i < hub->num_coders && hub->simulation_running)
		{
			if (check_burnout(hub->coders[i]))
				break ;
			i++;
		}
		if (!hub->simulation_running || check_all_compiles(hub))
			break ;
		usleep(1000);
	}
	i = -1;
	while (++i < hub->num_coders)
	{
		pthread_mutex_lock(&hub->dongles[i]->mutex);
		pthread_cond_broadcast(&hub->dongles[i]->cond);
		pthread_mutex_unlock(&hub->dongles[i]->mutex);
	}
	return (NULL);
}
