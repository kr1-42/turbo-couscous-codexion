/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_sim.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

static t_coder	**init_coders(long long number_of_coders)
{
	t_coder	**coders;
	long long	i;

	coders = (t_coder **)malloc(sizeof(t_coder *) * (number_of_coders + 1));
	if (!coders)
		return (NULL);
	i = 0;
	while (i < number_of_coders)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		if (!coders[i] || pthread_mutex_init(&coders[i]->mutex, NULL) != 0)
		{
			if (coders[i])
				free(coders[i]);
			coders[i] = NULL;
			return (cleanup_coders(coders, i), NULL);
		}
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		coders[i]->last_compile_start = 0;
		coders[i]->deadline = 0;
		coders[i]->seq = 0;
		coders[i]->waiting = 0;
		coders[i]->is_burned_out = 0;
		coders[i]->dongle_left = NULL;
		coders[i]->dongle_right = NULL;
		i++;
	}
	coders[number_of_coders] = NULL;
	return (coders);
}

static t_dongle	**init_dongles(t_coder **coders, long long number_of_coders)
{
	t_dongle	**dongles;
	long long	i;

	dongles = (t_dongle **)malloc(sizeof(t_dongle *) * (number_of_coders + 1));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i < number_of_coders)
	{
		dongles[i] = (t_dongle *)malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (dongles[i] = NULL, cleanup_dongles(dongles, i), NULL);
		dongles[i]->id = i + 1;
		dongles[i]->is_available = 1;
		dongles[i]->cooldown_until = 0;
		dongles[i]->owner_a = coders[i];
		dongles[i]->owner_b = coders[(i + 1) % number_of_coders];
		coders[i]->dongle_right = dongles[i];
		coders[(i + 1) % number_of_coders]->dongle_left = dongles[i];
		i++;
	}
	dongles[number_of_coders] = NULL;
	return (dongles);
}

static t_sim_state	*init_sim_state(void)
{
	t_sim_state	*state;

	state = (t_sim_state *)malloc(sizeof(t_sim_state));
	if (!state)
		return (NULL);
	if (pthread_mutex_init(&state->print_lock, NULL) != 0)
		return (free(state), NULL);
	if (pthread_mutex_init(&state->arbiter_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&state->print_lock), free(state), NULL);
	if (pthread_cond_init(&state->arbiter_cond, NULL) != 0)
	{
		pthread_mutex_destroy(&state->print_lock);
		pthread_mutex_destroy(&state->arbiter_mutex);
		return (free(state), NULL);
	}
	state->simulation_running = 1;
	state->next_seq = 0;
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
	simulation->dongles = init_dongles(simulation->coders,
			data->number_of_coders);
	if (!simulation->dongles)
		return (cleanup_coders(simulation->coders, data->number_of_coders),
			free(simulation), NULL);
	simulation->state = init_sim_state();
	if (!simulation->state)
		return (cleanup_coders(simulation->coders, data->number_of_coders),
			cleanup_dongles(simulation->dongles, data->number_of_coders),
			free(simulation), NULL);
	simulation->heap = heap_create(data->number_of_coders, data->edf_mode);
	if (!simulation->heap)
	{
		cleanup_coders(simulation->coders, data->number_of_coders);
		cleanup_dongles(simulation->dongles, data->number_of_coders);
		pthread_mutex_destroy(&simulation->state->print_lock);
		pthread_mutex_destroy(&simulation->state->arbiter_mutex);
		pthread_cond_destroy(&simulation->state->arbiter_cond);
		free(simulation->state);
		return (free(simulation), NULL);
	}
	simulation->start_time = 0;
	return (simulation);
}
