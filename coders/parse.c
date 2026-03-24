/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:17 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/18 20:29:48 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	parse_positive_int(const char *str)
{
	int			i;
	long long	result;

	if (!str || !str[0])
		return (-1);
	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (-1);
	result = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	return ((int)result);
}

int	parse_args(t_hub *hub, int argc, char **argv)
{
	if (argc != 9)
		return (printf("Error: Invalid number of arguments.\n"), 1);
	hub->num_coders = parse_positive_int(argv[1]);
	hub->time_to_burnout = parse_positive_int(argv[2]);
	hub->time_to_compile = parse_positive_int(argv[3]);
	hub->time_to_debug = parse_positive_int(argv[4]);
	hub->time_to_refactor = parse_positive_int(argv[5]);
	hub->required_compiles = parse_positive_int(argv[6]);
	hub->dongle_cooldown = parse_positive_int(argv[7]);
	if (hub->num_coders <= 0 || hub->time_to_burnout < 0
		|| hub->time_to_compile < 0 || hub->time_to_debug < 0
		|| hub->time_to_refactor < 0 || hub->required_compiles < 0
		|| hub->dongle_cooldown < 0)
		return (
			printf("Error: Arguments must be valid positive integers.\n"), 1);
	if (strcmp(argv[8], "fifo") == 0)
		hub->scheduler_type = 0;
	else if (strcmp(argv[8], "edf") == 0)
		hub->scheduler_type = 1;
	else
		return (
			printf("Error: Scheduler must be exactly 'fifo' or 'edf'.\n"), 1);
	hub->simulation_running = 1;
	return (0);
}
