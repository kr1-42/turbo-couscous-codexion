/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_release.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

void	release_both_dongles(t_simulation *sim, t_coder *coder)
{
	t_sim_state	*st;
	long long	until;

	st = sim->state;
	pthread_mutex_lock(&st->arbiter_mutex);
	until = sim_now(sim) + sim->args->dongle_cooldown;
	coder->dongle_left->is_available = 1;
	coder->dongle_left->cooldown_until = until;
	coder->dongle_right->is_available = 1;
	coder->dongle_right->cooldown_until = until;
	pthread_cond_broadcast(&st->arbiter_cond);
	pthread_mutex_unlock(&st->arbiter_mutex);
}
