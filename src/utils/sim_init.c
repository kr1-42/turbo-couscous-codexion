/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:53:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/19 17:32:48 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

t_simulation	*init_simulation(t_args *data)
{
	t_simulation	*simulation;

	simulation = fill_simulation(data);
	if (!simulation)
		return (NULL);
	simulation->start_time = get_current_time();
	return (simulation);
}
