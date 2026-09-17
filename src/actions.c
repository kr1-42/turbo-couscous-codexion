/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"
#include <stdio.h>
#include <time.h>

long long	sim_now(t_simulation *sim)
{
	return (get_current_time() - sim->start_time);
}

void	log_state(t_simulation *sim,
				long long ts, long long id, const char *msg)
{
	pthread_mutex_lock(&sim->state->print_lock);
	printf("%lld %lld %s\n", ts, id, msg);
	pthread_mutex_unlock(&sim->state->print_lock);
}

/* ==================== DONGLE HELPERS ==================== */

static int	dongle_free(t_dongle *d, long long now)
{
	return (d->is_available && now >= d->cooldown_until);
}

/*
** Returns 1 if `self` has priority over `other` for a dongle they both
** currently want, according to the active scheduler.
*/
static int	coder_wins(t_simulation *sim, t_coder *self, t_coder *other)
{
	if (sim->args->edf_mode && self->deadline != other->deadline)
		return (self->deadline < other->deadline);
	return (self->seq < other->seq);
}

static t_coder	*other_owner(t_dongle *d, t_coder *self)
{
	if (d->owner_a == self)
		return (d->owner_b);
	return (d->owner_a);
}

/* Both dongles a coder needs are currently free (ignoring priority). */
static int	coder_ready(t_coder *coder, long long now)
{
	if (coder->dongle_left == coder->dongle_right)
		return (0);
	return (dongle_free(coder->dongle_left, now)
		&& dongle_free(coder->dongle_right, now));
}

/*
** self loses the contention for dongle `d` only if the coder on the other
** side of it is a genuine rival: also waiting, AND itself ready to acquire
** both of its dongles right now (otherwise there is no real conflict to
** arbitrate, and self must not be blocked by a neighbor stuck elsewhere).
*/
static int	coder_wins_dongle(t_simulation *sim, t_coder *self,
		t_dongle *d, long long now)
{
	t_coder	*other;

	other = other_owner(d, self);
	if (other == self || !other->waiting || !coder_ready(other, now))
		return (1);
	return (coder_wins(sim, self, other));
}

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

/* ==================== ACQUIRE / RELEASE ==================== */

int	acquire_both_dongles(t_simulation *sim, t_coder *coder)
{
	t_sim_state		*st;
	struct timespec	ts;
	long long		now;

	st = sim->state;
	pthread_mutex_lock(&st->arbiter_mutex);
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

/* ==================== ACTION: COMPILE / DEBUG / REFACTOR ==================== */

void	action_compile(t_simulation *sim, t_coder *coder)
{
	log_state(sim, sim_now(sim), coder->id, "is compiling");
	action_sleep(sim->args->time_to_compile);
}

void	action_debug(t_simulation *sim, t_coder *coder)
{
	log_state(sim, sim_now(sim), coder->id, "is debugging");
	action_sleep(sim->args->time_to_debug);
}

void	action_refactor(t_simulation *sim, t_coder *coder)
{
	log_state(sim, sim_now(sim), coder->id, "is refactoring");
	action_sleep(sim->args->time_to_refactor);
}

/* ==================== ACTION: SLEEP ==================== */

void	action_sleep(long long milliseconds)
{
	if (milliseconds > 0)
		usleep(milliseconds * 1000);
}
