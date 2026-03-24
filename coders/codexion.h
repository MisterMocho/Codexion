/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luida-cu <luida-cu@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 20:18:13 by luida-cu          #+#    #+#             */
/*   Updated: 2026/03/24 20:19:24 by luida-cu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

/* ** Forward declarations so our structs can reference each other 
*/
typedef struct s_hub	t_hub;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
/*
** Data Structures
*/
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_held;
	long long		available_at; // Timestamp for when the cooldown expires
	t_coder			**queue;
	int				queue_size;
}	t_dongle;

typedef struct s_coder
{
	int				id; // 1 to N
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long long		last_compile_start;
	long long		enqueue_time;
	int				compiles_done;
	pthread_t		thread_id;
	pthread_mutex_t	coder_mutex;
	t_hub			*hub; // Pointer back to the shared data
	struct s_coder	*prev; // Doubly linked list pointer
	struct s_coder	*next; // Doubly linked list pointer
}	t_coder;

struct s_hub
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				required_compiles;
	long long		dongle_cooldown;
	int				scheduler_type; // e.g., 0 for FIFO, 1 for EDF
	long long		start_time;
	int				simulation_running;
	pthread_mutex_t	print_mutex; // Prevents logs from mixing
	t_dongle		**dongles; // Array of pointers to dongles
	t_coder			**coders; // Array of pointers to coders
};

int			parse_args(t_hub *hub, int argc, char **argv);
int			init_simulation(t_hub *hub);
int			start_simulation(t_hub *hub);
void		clean_simulation(t_hub *hub);
void		ft_usleep(long long time_in_ms);
void		print_status(t_coder *coder, const char *status);
void		request_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
void		enqueue_coder(t_dongle *dongle, t_coder *coder);
void		dequeue_coder(t_dongle *dongle);
void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);
t_coder		*peek_coder(t_dongle *dongle);
long long	get_time_in_ms(void);

#endif
