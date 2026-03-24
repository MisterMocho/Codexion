/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 17:58:18 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/20 19:03:38 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	signal_dongle_free(t_dongle *dongle)
{
	pthread_cond_broadcast(&dongle->cond);
}

void	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;
	long long		now;

	pthread_mutex_lock(&dongle->mutex);
	enqueue_coder(dongle, coder);
	while (coder->hub->simulation_running)
	{
		now = get_time_in_ms();
		if (!dongle->is_held && peek_coder(dongle) == coder)
		{
			if (now >= dongle->available_at)
				break ;
			ts.tv_sec = dongle->available_at / 1000;
			ts.tv_nsec = (dongle->available_at % 1000) * 1000000;
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	if (coder->hub->simulation_running)
	{
		dongle->is_held = 1;
		dequeue_coder(dongle);
	}
	pthread_mutex_unlock(&dongle->mutex);
}

void	request_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left_dongle < coder->right_dongle)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	wait_for_dongle(coder, first);
	print_status(coder, "has taken a dongle");
	wait_for_dongle(coder, second);
	print_status(coder, "has taken a dongle");
}

void	release_dongles(t_coder *coder)
{
	long long	current_time;

	current_time = get_time_in_ms();
	pthread_mutex_lock(&coder->left_dongle->mutex);
	coder->left_dongle->is_held = 0;
	coder->left_dongle->available_at = (current_time
			+ coder->hub->dongle_cooldown);
	signal_dongle_free(coder->left_dongle);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_lock(&coder->right_dongle->mutex);
	coder->right_dongle->is_held = 0;
	coder->right_dongle->available_at = (current_time
			+ coder->hub->dongle_cooldown);
	signal_dongle_free(coder->right_dongle);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}
