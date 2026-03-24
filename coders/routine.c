/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 17:17:09 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 20:39:49 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*one_coder_routine(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	print_status(coder, "has taken a dongle");
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	while (coder->hub->simulation_running)
		usleep(500);
	return (NULL);
}

static void	coder_loop(t_coder *coder)
{
	while (coder->hub->simulation_running)
	{
		request_dongles(coder);
		print_status(coder, "is compiling");
		pthread_mutex_lock(&coder->coder_mutex);
		coder->last_compile_start = get_time_in_ms();
		pthread_mutex_unlock(&coder->coder_mutex);
		ft_usleep(coder->hub->time_to_compile);
		release_dongles(coder);
		pthread_mutex_lock(&coder->coder_mutex);
		coder->compiles_done++;
		if (coder->hub->required_compiles > 0
			&& coder->compiles_done >= coder->hub->required_compiles)
		{
			pthread_mutex_unlock(&coder->coder_mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->coder_mutex);
		print_status(coder, "is debugging");
		ft_usleep(coder->hub->time_to_debug);
		print_status(coder, "is refactoring");
		ft_usleep(coder->hub->time_to_refactor);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->hub->num_coders == 1)
		return (one_coder_routine(coder));
	if (coder->id % 2 == 0)
		ft_usleep(10);
	coder_loop(coder);
	return (NULL);
}
