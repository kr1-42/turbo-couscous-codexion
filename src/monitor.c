/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

static void	stop_simulation(t_simulation *sim)
{
	pthread_mutex_lock(&sim->state->arbiter_mutex);
	sim->state->simulation_running = 0;
	pthread_cond_broadcast(&sim->state->arbiter_cond);
	pthread_mutex_unlock(&sim->state->arbiter_mutex);
}

static long long	check_burnout(t_simulation *sim)
{
	long long	i;
	long long	now;

	now = sim_now(sim);
	i = 0;
	while (sim->coders[i])
	{
		pthread_mutex_lock(&sim->coders[i]->mutex);
		if (!sim->coders[i]->is_burned_out
			&& sim->coders[i]->compile_count
				< sim->args->number_of_compiles_required
			&& now - sim->coders[i]->last_compile_start
				>= sim->args->time_to_burnout)
		{
			sim->coders[i]->is_burned_out = 1;
			pthread_mutex_unlock(&sim->coders[i]->mutex);
			return (sim->coders[i]->id);
		}
		pthread_mutex_unlock(&sim->coders[i]->mutex);
		i++;
	}
	return (0);
}

static int	all_coders_done(t_simulation *sim)
{
	long long	i;
	int			done;

	i = 0;
	done = 1;
	while (sim->coders[i])
	{
		pthread_mutex_lock(&sim->coders[i]->mutex);
		if (sim->coders[i]->compile_count
			< sim->args->number_of_compiles_required)
			done = 0;
		pthread_mutex_unlock(&sim->coders[i]->mutex);
		i++;
	}
	return (done);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	long long		burned_id;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->state->arbiter_mutex);
		if (!sim->state->simulation_running)
		{
			pthread_mutex_unlock(&sim->state->arbiter_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state->arbiter_mutex);
		burned_id = check_burnout(sim);
		if (burned_id)
		{
			log_state(sim, sim_now(sim), burned_id, "burned out");
			stop_simulation(sim);
			break ;
		}
		if (all_coders_done(sim))
		{
			stop_simulation(sim);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
