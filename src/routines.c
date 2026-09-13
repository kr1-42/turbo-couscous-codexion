/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

static int	simulation_is_running(t_simulation *sim)
{
	int	running;

	pthread_mutex_lock(&sim->state->arbiter_mutex);
	running = sim->state->simulation_running;
	pthread_mutex_unlock(&sim->state->arbiter_mutex);
	return (running);
}

static int	coder_is_done(t_simulation *sim, t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&coder->mutex);
	done = coder->compile_count >= sim->args->number_of_compiles_required;
	pthread_mutex_unlock(&coder->mutex);
	return (done);
}

/* ==================== CODER MAIN ROUTINE ==================== */

void	*coder_routine(void *arg)
{
	t_thread_context	*ctx;
	t_simulation		*sim;
	t_coder				*coder;

	ctx = (t_thread_context *)arg;
	if (!ctx || !ctx->sim || !ctx->coder)
		return (NULL);
	sim = ctx->sim;
	coder = ctx->coder;
	free(ctx);
	while (simulation_is_running(sim) && !coder_is_done(sim, coder))
	{
		if (!acquire_both_dongles(sim, coder))
			break ;
		pthread_mutex_lock(&coder->mutex);
		coder->last_compile_start = sim_now(sim);
		pthread_mutex_unlock(&coder->mutex);
		action_compile(sim, coder);
		release_both_dongles(sim, coder);
		if (!simulation_is_running(sim))
			break ;
		action_debug(sim, coder);
		if (!simulation_is_running(sim))
			break ;
		action_refactor(sim, coder);
		pthread_mutex_lock(&coder->mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->mutex);
	}
	return (NULL);
}

/* ==================== THREAD LAUNCHER ==================== */

int	launch_coder_threads(t_simulation *sim, pthread_t **threads)
{
	long long			i;
	t_thread_context	*ctx;

	if (!sim || !sim->coders)
		return (0);
	*threads = (pthread_t *)malloc(sizeof(pthread_t)
			* sim->args->number_of_coders);
	if (!*threads)
		return (0);
	i = 0;
	while (sim->coders[i])
	{
		ctx = (t_thread_context *)malloc(sizeof(t_thread_context));
		if (!ctx)
			return (err_msg(MALLOC_ERROR), 0);
		ctx->sim = sim;
		ctx->coder = sim->coders[i];
		if (pthread_create(&(*threads)[i], NULL, coder_routine,
				(void *)ctx) != 0)
		{
			free(ctx);
			err_msg(MALLOC_ERROR);
			return (0);
		}
		i++;
	}
	return (1);
}

/* ==================== THREAD JOINER ==================== */

int	join_coder_threads(t_simulation *sim, pthread_t *threads)
{
	long long	i;

	if (!threads)
		return (0);
	i = 0;
	while (i < sim->args->number_of_coders)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	free(threads);
	return (1);
}
