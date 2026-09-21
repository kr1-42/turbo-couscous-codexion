/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

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
			return (free(ctx), err_msg(MALLOC_ERROR), 0);
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
