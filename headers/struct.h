/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 12:39:08 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <pthread.h>

typedef struct s_simulation		t_simulation;
typedef struct s_coder			t_coder;
typedef struct s_dongle			t_dongle;

/* ==================== HEAP (priority queue) ==================== */

typedef struct s_heap_node
{
	void			*data;
	long long		key1;
	long long		key2;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node		*nodes;
	long long		size;
	long long		capacity;
	int				edf_mode;
}	t_heap;

/* ==================== DONGLE STRUCTURE ==================== */

struct s_dongle
{
	long long	id;
	int			is_available;
	long long	cooldown_until;
	t_coder		*owner_a;
	t_coder		*owner_b;
};

/* ==================== CODER STRUCTURE ==================== */

struct s_coder
{
	long long		id;
	long long		compile_count;
	long long		last_compile_start;
	long long		deadline;
	long long		seq;
	int				waiting;
	int				is_burned_out;
	t_dongle		*dongle_left;
	t_dongle		*dongle_right;
	pthread_mutex_t	mutex;
};

/* ==================== SIMULATION ARGS ==================== */

typedef struct s_args
{
	long long	number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	long long	number_of_compiles_required;
	long long	dongle_cooldown;
	char		*scheduler;
	int			edf_mode;
}	t_args;

/* ==================== SIMULATION STATE MANAGER ==================== */

typedef struct s_sim_state
{
	pthread_mutex_t	print_lock;
	pthread_mutex_t	arbiter_mutex;
	pthread_cond_t	arbiter_cond;
	long long		simulation_running;
	long long		next_seq;
}	t_sim_state;

/* ==================== THREAD CONTEXT ==================== */

typedef struct s_thread_context
{
	t_simulation	*sim;
	t_coder			*coder;
}	t_thread_context;

/* ==================== MAIN SIMULATION STRUCTURE ==================== */

struct s_simulation
{
	t_args			*args;
	t_coder			**coders;
	t_dongle		**dongles;
	t_sim_state		*state;
	t_heap			*heap;
	long long		start_time;
};

#endif
