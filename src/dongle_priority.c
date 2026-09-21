/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_priority.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

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
int	coder_ready(t_coder *coder, long long now)
{
	if (coder->dongle_left == coder->dongle_right)
		return (0);
	return (dongle_free(coder->dongle_left, now)
		&& dongle_free(coder->dongle_right, now));
}

/*m
** self loses the contention for dongle `d` only if the coder on the other
** side of it is a genuine rival: also waiting, AND itself ready to acquire
** both of its dongles right now (otherwise there is no real conflict to
** arbitrate, and self must not be blocked by a neighbor stuck elsewhere).
*/
int	coder_wins_dongle(t_simulation *sim, t_coder *self,
		t_dongle *d, long long now)
{
	t_coder	*other;

	other = other_owner(d, self);
	if (other == self || !other->waiting || !coder_ready(other, now))
		return (1);
	return (coder_wins(sim, self, other));
}
