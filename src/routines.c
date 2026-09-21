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

static int	coder_cycle(t_simulation *sim, t_coder *coder)
{
	if (!acquire_both_dongles(sim, coder))
		return (0);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_start = sim_now(sim);
	pthread_mutex_unlock(&coder->mutex);
	action_compile(sim, coder);
	release_both_dongles(sim, coder);
	if (!simulation_is_running(sim))
		return (0);
	action_debug(sim, coder);
	if (!simulation_is_running(sim))
		return (0);
	action_refactor(sim, coder);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->mutex);
	return (1);
}

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
		if (!coder_cycle(sim, coder))
			break ;
	return (NULL);
}
