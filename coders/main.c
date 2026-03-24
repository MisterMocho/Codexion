/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:06 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/18 21:22:40 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_hub	hub;

	memset(&hub, 0, sizeof(t_hub));
	if (parse_args(&hub, argc, argv) != 0)
		return (1);
	if (init_simulation(&hub) != 0)
	{
		clean_simulation(&hub);
		return (1);
	}
	if (start_simulation(&hub) != 0)
	{
		clean_simulation(&hub);
		return (1);
	}
	clean_simulation(&hub);
	return (0);
}
