/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:06:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/19 17:25:47 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"
#include <stdlib.h>
#include <time.h>

static t_coder	**init_coders(int number_of_coders)
{
	t_coder	**coders;
	int		i;

	coders = (t_coder **)malloc(sizeof(t_coder *) * (number_of_coders + 1));
	if (!coders)
		return (NULL);
	i = 0;
	while (i++ < number_of_dongles)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		if (!coders[i])
			return (cleanup_coders(coders), NULL);
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		coders[i]->last_compile_time = 0;
		coders[i]->burnout_time = 0;
		coders[i]->is_burned_out = 0;
		if (pthread_mutex_init(&coders[i]->mutex, NULL) != 0)
			return (cleanup_coders(coders), NULL);
	}
	coders[number_of_coders] = NULL;
	return (coders);
}

static t_dongle	**init_dongles(int number_of_dongles)
{
	t_dongle	**dongles;
	int			i;

	dongles = (t_dongle **)malloc(sizeof(t_dongle *) * (number_of_dongles + 1));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i++ < number_of_dongles)
	{
		dongles[i] = (t_dongle *)malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (cleanup_dongles(dongles), NULL);
		if (pthread_mutex_init(&dongles[i]->mutex, NULL) != 0)
			return (cleanup_dongles(dongles), NULL);
		dongles[i]->id = i + 1;
		dongles[i]->is_available = 1;
		dongles[i]->cooldown_time = 0;
	}
	dongles[number_of_dongles] = NULL;
	return (dongles);
}

static t_sim_state	*init_sim_state(void)
{
	t_sim_state	*state;

	state = (t_sim_state *)malloc(sizeof(t_sim_state));
	if (!state)
		return (NULL);
	if (pthread_mutex_init(&state->global_lock, NULL) != 0)
	{
		free(state);
		return (NULL);
	}
	if (pthread_mutex_init(&state->print_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&state->global_lock);
		free(state);
		return (NULL);
	}
	state->simulation_running = 1;
	state->total_compiles = 0;
	return (state);
}

t_simulation	*fill_simulation(t_args *data)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)malloc(sizeof(t_simulation));
	if (!simulation)
		return (NULL);
	simulation->args = data;
	simulation->coders = init_coders(data->number_of_coders);
	if (!simulation->coders)
		return (free(simulation), NULL);
	simulation->dongles = init_dongles(data->number_of_coders);
	if (!simulation->dongles)
		return (cleanup_coders(simulation->coders), free(simulation), NULL);
	simulation->state = init_sim_state();
	if (!simulation->state)
		return (cleanup_coders(simulation->coders), free(simulation->dongles), \
			free(simulation), NULL);
	simulation->job_queue = queue_create();
	if (!simulation->job_queue)
		return (cleanup_coders(simulation->coders), free(simulation->dongles), \
			pthread_mutex_destroy(&simulation->state->global_lock), \
			pthread_mutex_destroy(&simulation->state->print_lock), \
			free(simulation->state), free(simulation), NULL);
	simulation->start_time = 0;
	return (simulation);
}
