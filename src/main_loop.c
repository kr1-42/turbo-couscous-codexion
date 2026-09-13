/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

static int	data_dispatch(t_args *data)
{
	if (!data)
		return (err_msg(MALLOC_ERROR), 0);
	if (data == (t_args *)1)
		return (err_msg(ARG_ERROR), 0);
	if (data == (t_args *)2)
		return (err_msg(SCHEDULER_ERROR), 0);
	return (1);
}

static int	run_simulation(t_simulation *sim)
{
	pthread_t	*threads;
	pthread_t	monitor;

	if (!sim)
		return (-1);
	sim->start_time = get_current_time();
	threads = NULL;
	if (!launch_coder_threads(sim, &threads))
		return (-1);
	if (pthread_create(&monitor, NULL, monitor_routine, (void *)sim) != 0)
	{
		join_coder_threads(sim, threads);
		return (-1);
	}
	join_coder_threads(sim, threads);
	pthread_join(monitor, NULL);
	return (0);
}

int	main_loop(char **av)
{
	t_args			*data;
	t_simulation	*simulation;

	data = parse_data(av);
	if (!data_dispatch(data))
		return (-1);
	simulation = init_simulation(data);
	if (!simulation)
		return (free(data), err_msg(MALLOC_ERROR), -1);
	if (run_simulation(simulation) == -1)
		return (free_simulation(simulation), err_msg(SIM_ERR), -1);
	free_simulation(simulation);
	return (0);
}
