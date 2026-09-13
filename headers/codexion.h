/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include "struct.h"

# define N_ARG_ERROR "wrong number of arguments, see subject for usage\n"
# define SCHEDULER_ERROR "scheduler must be exactly fifo or edf\n"
# define ARG_ERROR "arguments must be valid integers (coders >= 1)\n"
# define MALLOC_ERROR "malloc error occured\n"
# define SIM_ERR "simulation error occured\n"

/* context */

void			err_msg(const char *msg);

/* utils */
long long		get_current_time(void);
int				ft_strlen(const char *str);
long long		ft_atoll(const char *number_str);
int				ft_strcmp(const char *s1, const char *s2);
t_simulation	*fill_simulation(t_args *data);
t_args			*parse_data(char **av);

/* cleanup */
void			cleanup_coders(t_coder **coders, long long n);
void			cleanup_dongles(t_dongle **dongles, long long n);
void			free_simulation(t_simulation *sim);

/* heap (priority queue) */
t_heap			*heap_create(long long capacity, int edf_mode);
void			heap_push(t_heap *h, void *data, long long key1,
					long long key2);
void			*heap_remove(t_heap *h, void *data);
void			heap_destroy(t_heap *h);

/* simulation */
int				main_loop(char **av);
t_simulation	*init_simulation(t_args *data);

/* actions */
long long		sim_now(t_simulation *sim);
void			log_state(t_simulation *sim, long long ts, long long id,
					const char *msg);
int				acquire_both_dongles(t_simulation *sim, t_coder *coder);
void			release_both_dongles(t_simulation *sim, t_coder *coder);
void			action_compile(t_simulation *sim, t_coder *coder);
void			action_debug(t_simulation *sim, t_coder *coder);
void			action_refactor(t_simulation *sim, t_coder *coder);
void			action_sleep(long long milliseconds);

/* routines */
void			*coder_routine(void *arg);
int				launch_coder_threads(t_simulation *sim, pthread_t **threads);
int				join_coder_threads(t_simulation *sim, pthread_t *threads);

/* monitor */
void			*monitor_routine(void *arg);

#endif
