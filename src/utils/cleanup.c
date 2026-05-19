/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:09:13 by chrilomb          #+#    #+#             */
/*   Updated: 2026/05/19 14:04:18 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"
#include <stdlib.h>

void	cleanup_coders(t_coder **coders)
{
	int	i;

	if (!coders)
		return ;
	i = 0;
	while (coders[i])
	{
		pthread_mutex_destroy(&coders[i]->mutex);
		free(coders[i]);
		i++;
	}
	free(coders);
}

static void	cleanup_dongles(t_dongle **dongles)
{
	int	i;

	if (!dongles)
		return ;
	i = 0;
	while (dongles[i])
	{
		pthread_mutex_destroy(&dongles[i]->mutex);
		free(dongles[i]);
		i++;
	}
	free(dongles);
}

void	free_simulation(t_simulation *sim)
{
	if (!sim)
		return ;
	if (sim->coders)
		cleanup_coders(sim->coders);
	if (sim->dongles)
		cleanup_dongles(sim->dongles);
	if (sim->job_queue)
		queue_destroy(sim->job_queue);
	if (sim->state)
	{
		pthread_mutex_destroy(&sim->state->global_lock);
		pthread_mutex_destroy(&sim->state->print_lock);
		free(sim->state);
	}
	if (sim->args)
		free(sim->args);
	free(sim);
}
