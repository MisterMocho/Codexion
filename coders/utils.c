/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 17:06:53 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 18:47:56 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, const char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&coder->hub->print_mutex);
	if (coder->hub->simulation_running == 1)
	{
		timestamp = get_time_in_ms() - coder->hub->start_time;
		printf("%lld %d %s\n", timestamp, coder->id, status);
	}
	pthread_mutex_unlock(&coder->hub->print_mutex);
}

t_coder	*peek_coder(t_dongle *dongle)
{
	if (dongle->queue_size == 0)
		return (NULL);
	return (dongle->queue[0]);
}
