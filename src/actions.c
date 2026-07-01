/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/01 15:05:41 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"
#include <unistd.h>
#include <stdio.h>

/* ==================== ACQUIRE DONGLE ==================== */

int	acquire_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle)
{
	int	acquired;

	pthread_mutex_lock(&dongle->mutex);
	acquired = 0;

	if (dongle->is_available && dongle->cooldown_time <= (get_current_time() - sim->start_time))
	{
		dongle->is_available = 0;
		acquired = 1;

		pthread_mutex_lock(&sim->state->print_lock);
		printf("[%lld ms] Coder %lld: Acquired dongle %lld\n",
			get_current_time() - sim->start_time, coder->id, dongle->id);
		pthread_mutex_unlock(&sim->state->print_lock);
	}

	pthread_mutex_unlock(&dongle->mutex);
	return (acquired);
}

/* ==================== RELEASE DONGLE ==================== */

void	release_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);

	dongle->is_available = 1;
	dongle->cooldown_time = get_current_time() - sim->start_time + sim->args->dongle_cooldown;

	pthread_mutex_unlock(&dongle->mutex);

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Released dongle %lld (cooldown until %lld ms)\n",
		get_current_time() - sim->start_time, coder->id, dongle->id, dongle->cooldown_time);
	pthread_mutex_unlock(&sim->state->print_lock);
}

/* ==================== ACTION: COMPILE ==================== */

void	action_compile(t_simulation *sim, t_coder *coder, t_dongle *dongle)
{
	long long	start_time;
	long long	end_time;

	start_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Starting compile (dongle %lld)...\n",
		start_time, coder->id, dongle->id);
	pthread_mutex_unlock(&sim->state->print_lock);

	/* Sleep for compile duration */
	action_sleep(sim->args->time_to_compile);

	end_time = get_current_time() - sim->start_time;

	/* Update coder state */
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile_time = end_time;
	pthread_mutex_unlock(&coder->mutex);

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Compile complete (took %lld ms)\n",
		end_time, coder->id, end_time - start_time);
	pthread_mutex_unlock(&sim->state->print_lock);
}

/* ==================== ACTION: DEBUG ==================== */

void	action_debug(t_simulation *sim, t_coder *coder)
{
	long long	start_time;
	long long	end_time;

	start_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Starting debug...\n", start_time, coder->id);
	pthread_mutex_unlock(&sim->state->print_lock);

	/* Sleep for debug duration */
	action_sleep(sim->args->time_to_debug);

	end_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Debug complete (took %lld ms)\n",
		end_time, coder->id, end_time - start_time);
	pthread_mutex_unlock(&sim->state->print_lock);
}

/* ==================== ACTION: REFACTOR ==================== */

void	action_refactor(t_simulation *sim, t_coder *coder)
{
	long long	start_time;
	long long	end_time;

	start_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Starting refactor...\n", start_time, coder->id);
	pthread_mutex_unlock(&sim->state->print_lock);

	/* Sleep for refactor duration */
	action_sleep(sim->args->time_to_refactor);

	end_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Refactor complete (took %lld ms)\n",
		end_time, coder->id, end_time - start_time);
	pthread_mutex_unlock(&sim->state->print_lock);
}

/* ==================== ACTION: CHECK BURNOUT ==================== */

int	action_check_burnout(t_simulation *sim, t_coder *coder)
{
	long long	current_time;
	long long	elapsed_time;
	int			is_burned_out;

	current_time = get_current_time() - sim->start_time;

	pthread_mutex_lock(&coder->mutex);

	/* If not already burned out, check time since this coder's last compile */
	if (!coder->is_burned_out)
	{
		elapsed_time = current_time - coder->last_compile_time;
		if (elapsed_time >= sim->args->time_to_burnout)
		{
			coder->is_burned_out = 1;
			coder->burnout_time = current_time;
		}
	}

	is_burned_out = coder->is_burned_out;
	pthread_mutex_unlock(&coder->mutex);

	return (is_burned_out);
}

/* ==================== ACTION: SLEEP/WAIT ==================== */

void	action_sleep(long long milliseconds)
{
	/* TODO: Implement accurate sleep */
	/* - Convert milliseconds to microseconds */
	/* - Use usleep() */
	usleep(milliseconds * 1000);
}
