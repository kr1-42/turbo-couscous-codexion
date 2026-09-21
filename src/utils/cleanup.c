/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

void	cleanup_coders(t_coder **coders, long long n)
{
	long long	i;

	if (!coders)
		return ;
	i = 0;
	while (i < n && coders[i])
	{
		pthread_mutex_destroy(&coders[i]->mutex);
		free(coders[i]);
		i++;
	}
	free(coders);
}

void	cleanup_dongles(t_dongle **dongles, long long n)
{
	long long	i;

	if (!dongles)
		return ;
	i = 0;
	while (i < n && dongles[i])
	{
		free(dongles[i]);
		i++;
	}
	free(dongles);
}

void	cleanup_partial_sim(t_simulation *sim, int stage)
{
	if (stage >= 3)
	{
		pthread_mutex_destroy(&sim->state->print_lock);
		pthread_mutex_destroy(&sim->state->arbiter_mutex);
		pthread_cond_destroy(&sim->state->arbiter_cond);
		free(sim->state);
	}
	if (stage >= 2)
		cleanup_dongles(sim->dongles, sim->args->number_of_coders);
	if (stage >= 1)
		cleanup_coders(sim->coders, sim->args->number_of_coders);
	free(sim);
}

void	free_simulation(t_simulation *sim)
{
	if (!sim)
		return ;
	if (sim->coders)
		cleanup_coders(sim->coders, sim->args->number_of_coders);
	if (sim->dongles)
		cleanup_dongles(sim->dongles, sim->args->number_of_coders);
	if (sim->heap)
		heap_destroy(sim->heap);
	if (sim->state)
	{
		pthread_mutex_destroy(&sim->state->print_lock);
		pthread_mutex_destroy(&sim->state->arbiter_mutex);
		pthread_cond_destroy(&sim->state->arbiter_cond);
		free(sim->state);
	}
	if (sim->args)
		free(sim->args);
	free(sim);
}
