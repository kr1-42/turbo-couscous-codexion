/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 20:49:32 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
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
void			log_state(t_simulation *sim, long long ts, long long id,
					const char *msg);

/* utils */
long long		get_current_time(void);
long long		sim_now(t_simulation *sim);
int				ft_strlen(const char *str);
long long		ft_atoll(const char *number_str);
int				ft_strcmp(const char *s1, const char *s2);
t_simulation	*fill_simulation(t_args *data);
t_args			*parse_data(char **av);
void			*ft_calloc(size_t num_elems, size_t byte_size);

/* cleanup */
void			cleanup_coders(t_coder **coders, long long n);
void			cleanup_dongles(t_dongle **dongles, long long n);
void			cleanup_partial_sim(t_simulation *sim, int stage);
void			free_simulation(t_simulation *sim);

/* heap (priority queue) */
void			sift_up(t_heap *h, long long i);
t_heap			*heap_create(long long capacity, int edf_mode);
void			heap_push(t_heap *h, void *data, long long key1,
					long long key2);
void			*heap_remove(t_heap *h, void *data);
void			heap_destroy(t_heap *h);

/* simulation */
int				main_loop(char **av);
t_simulation	*init_simulation(t_args *data);

/* dongle arbitration */
int				coder_ready(t_coder *coder, long long now);
int				coder_wins_dongle(t_simulation *sim, t_coder *self,
					t_dongle *d, long long now);
int				acquire_both_dongles(t_simulation *sim, t_coder *coder);
void			release_both_dongles(t_simulation *sim, t_coder *coder);

/* actions */
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
