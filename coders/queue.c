/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/20 19:08:54 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 19:04:14 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	has_higher_priority(t_coder *a, t_coder *b)
{
	long long	a_dl;
	long long	b_dl;

	a_dl = a->last_compile_start + a->hub->time_to_burnout;
	b_dl = b->last_compile_start + b->hub->time_to_burnout;
	if (a->hub->scheduler_type == 0 || a_dl == b_dl)
		return (a->enqueue_time < b->enqueue_time);
	return (a_dl < b_dl);
}

static void	heapify_up(t_dongle *dongle, int index)
{
	int		parent;
	t_coder	*temp;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (has_higher_priority(dongle->queue[index], dongle->queue[parent]))
		{
			temp = dongle->queue[index];
			dongle->queue[index] = dongle->queue[parent];
			dongle->queue[parent] = temp;
			index = parent;
		}
		else
			break ;
	}
}

static void	heapify_down(t_dongle *d, int i)
{
	int		sm;
	int		l;
	int		r;
	t_coder	*tmp;

	while (1)
	{
		sm = i;
		l = 2 * i + 1;
		r = 2 * i + 2;
		if (l < d->queue_size)
			if (has_higher_priority(d->queue[l], d->queue[sm]))
				sm = l;
		if (r < d->queue_size)
			if (has_higher_priority(d->queue[r], d->queue[sm]))
				sm = r;
		if (sm == i)
			break ;
		tmp = d->queue[i];
		d->queue[i] = d->queue[sm];
		d->queue[sm] = tmp;
		i = sm;
	}
}

void	enqueue_coder(t_dongle *dongle, t_coder *coder)
{
	coder->enqueue_time = get_time_in_ms();
	dongle->queue[dongle->queue_size] = coder;
	heapify_up(dongle, dongle->queue_size);
	dongle->queue_size++;
}

void	dequeue_coder(t_dongle *dongle)
{
	if (dongle->queue_size == 0)
		return ;
	dongle->queue[0] = dongle->queue[dongle->queue_size - 1];
	dongle->queue_size--;
	if (dongle->queue_size > 0)
		heapify_down(dongle, 0);
}
