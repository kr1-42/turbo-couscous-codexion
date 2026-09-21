/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_acquire.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"
#include <time.h>

static int	coder_can_acquire(t_simulation *sim, t_coder *coder, long long now)
{
	if (!coder_ready(coder, now))
		return (0);
	return (coder_wins_dongle(sim, coder, coder->dongle_left, now)
		&& coder_wins_dongle(sim, coder, coder->dongle_right, now));
}

static long long	next_wakeup(t_coder *coder, long long now)
{
	long long	earliest;

	earliest = now + 50;
	if (!coder->dongle_left->is_available
		|| coder->dongle_left->cooldown_until > now)
	{
		if (coder->dongle_left->cooldown_until < earliest)
			earliest = coder->dongle_left->cooldown_until;
	}
	if (!coder->dongle_right->is_available
		|| coder->dongle_right->cooldown_until > now)
	{
		if (coder->dongle_right->cooldown_until < earliest)
			earliest = coder->dongle_right->cooldown_until;
	}
	if (earliest <= now)
		earliest = now + 1;
	return (earliest);
}

static void	build_timeout(long long delta_ms, struct timespec *ts)
{
	clock_gettime(CLOCK_REALTIME, ts);
	if (delta_ms < 0)
		delta_ms = 0;
	ts->tv_sec += delta_ms / 1000;
	ts->tv_nsec += (delta_ms % 1000) * 1000000;
	if (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec += 1;
		ts->tv_nsec -= 1000000000;
	}
}

static void	register_wait(t_simulation *sim, t_coder *coder, t_sim_state *st)
{
	struct timespec	ts;
	long long		now;

	coder->seq = st->next_seq++;
	coder->deadline = coder->last_compile_start + sim->args->time_to_burnout;
	coder->waiting = 1;
	heap_push(sim->heap, coder, coder->deadline, coder->seq);
	now = sim_now(sim);
	while (st->simulation_running && !coder_can_acquire(sim, coder, now))
	{
		build_timeout(next_wakeup(coder, now) - now, &ts);
		pthread_cond_timedwait(&st->arbiter_cond, &st->arbiter_mutex, &ts);
		now = sim_now(sim);
	}
	heap_remove(sim->heap, coder);
	coder->waiting = 0;
}

int	acquire_both_dongles(t_simulation *sim, t_coder *coder)
{
	t_sim_state	*st;
	long long	now;

	st = sim->state;
	pthread_mutex_lock(&st->arbiter_mutex);
	register_wait(sim, coder, st);
	if (!st->simulation_running)
	{
		pthread_mutex_unlock(&st->arbiter_mutex);
		return (0);
	}
	coder->dongle_left->is_available = 0;
	coder->dongle_right->is_available = 0;
	now = sim_now(sim);
	log_state(sim, now, coder->id, "has taken a dongle");
	log_state(sim, sim_now(sim), coder->id, "has taken a dongle");
	pthread_mutex_unlock(&st->arbiter_mutex);
	return (1);
}
