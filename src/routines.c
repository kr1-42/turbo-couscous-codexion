/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/05/26 15:02:27 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"
#include <stdio.h>
#include <stdlib.h>

/* ==================== CODER MAIN ROUTINE ==================== */

void	*coder_routine(void *arg)
{
	t_thread_context	*ctx;
	t_simulation		*sim;
	t_coder				*coder;
	t_dongle			*dongle;
	long long			i;
	int					dongle_acquired;

	ctx = (t_thread_context *)arg;
	if (!ctx || !ctx->sim || !ctx->coder)
		return (NULL);

	sim = ctx->sim;
	coder = ctx->coder;

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Started\n", get_current_time() - sim->start_time, coder->id);
	pthread_mutex_unlock(&sim->state->print_lock);

	/* Main work loop */
	while (1)
	{
		/* Check if simulation is still running */
		pthread_mutex_lock(&sim->state->global_lock);
		if (!sim->state->simulation_running)
		{
			pthread_mutex_unlock(&sim->state->global_lock);
			break ;
		}
		pthread_mutex_unlock(&sim->state->global_lock);

		/* Check if burned out */
		if (action_check_burnout(sim, coder))
		{
			pthread_mutex_lock(&sim->state->print_lock);
			printf("[%lld ms] Coder %lld: Burned out!\n",
				get_current_time() - sim->start_time, coder->id);
			pthread_mutex_unlock(&sim->state->print_lock);
			break ;
		}

		/* Check if reached required compiles */
        printf("Coder %lld compile count: %lld\n", coder->id, coder->compile_count);
		pthread_mutex_lock(&coder->mutex);
		if (coder->compile_count >= sim->args->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->mutex);
			break ;
		}
		pthread_mutex_unlock(&coder->mutex);

		/* Try to acquire a dongle */
		dongle_acquired = 0;
		i = 0;
		while (sim->dongles[i] && !dongle_acquired)
		{
			dongle = sim->dongles[i];
			if (acquire_dongle(sim, coder, dongle))
			{
				dongle_acquired = 1;
				break ;
			}
			i++;
		}

		/* If no dongle acquired, sleep and retry */
		if (!dongle_acquired)
		{
			action_sleep(1);
			continue ;
		}

		/* Execute compile cycle */
		action_compile(sim, coder, dongle);
		action_debug(sim, coder);
		action_refactor(sim, coder);

		/* Release dongle */
		release_dongle(sim, coder, dongle);

		/* Update global compile count */
		pthread_mutex_lock(&sim->state->global_lock);
		sim->state->total_compiles++;
		pthread_mutex_unlock(&sim->state->global_lock);
	}

	pthread_mutex_lock(&sim->state->print_lock);
	printf("[%lld ms] Coder %lld: Finished (Total compiles: %lld)\n",
		get_current_time() - sim->start_time, coder->id, coder->compile_count);
	pthread_mutex_unlock(&sim->state->print_lock);

	free(ctx);
	return (NULL);
}

/* ==================== THREAD LAUNCHER ==================== */

int	launch_coder_threads(t_simulation *sim, pthread_t **threads)
{
	long long			i;
	t_thread_context	*ctx;
	int					result;

	if (!sim || !sim->coders)
		return (0);

	*threads = (pthread_t *)malloc(sizeof(pthread_t) * (sim->args->number_of_coders + 1));
	if (!*threads)
		return (0);

	i = 0;
	while (sim->coders[i])
	{
		ctx = (t_thread_context *)malloc(sizeof(t_thread_context));
		if (!ctx)
		{
			while (i > 0)
			{
				i--;
				pthread_cancel((*threads)[i]);
			}
			free(*threads);
			return (0);
		}

		ctx->sim = sim;
		ctx->coder = sim->coders[i];

		result = pthread_create(&(*threads)[i], NULL, coder_routine, (void *)ctx);
		if (result != 0)
		{
			free(ctx);
			while (i > 0)
			{
				i--;
				pthread_cancel((*threads)[i]);
			}
			free(*threads);
			return (0);
		}

		i++;
	}
	(*threads)[i] = (pthread_t){0};  /* Null terminate */

	return (1);
}

/* ==================== THREAD JOINER ==================== */

int	join_coder_threads(t_simulation *sim, pthread_t *threads)
{
	long long	i;
	int			result;

	if (!threads)
		return (0);

	i = 0;
	while (i == 0 || *(unsigned long *)&threads[i] != 0)
	{
		/* Check if thread handle is valid (not all zeros) */
		if (i > 0 && *(unsigned long *)&threads[i] == 0)
			break ;

		result = pthread_join(threads[i], NULL);
		if (result != 0)
		{
			pthread_mutex_lock(&sim->state->print_lock);
			printf("Error joining thread %lld: %d\n", i, result);
			pthread_mutex_unlock(&sim->state->print_lock);
			return (0);
		}

		i++;
	}

	free(threads);
	return (1);
}

