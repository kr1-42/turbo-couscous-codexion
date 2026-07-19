/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:35:39 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/19 17:35:45 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"
#include <stdio.h>

static void	coder_add(t_coder **coders, int id)
{
	int i = 0;
	while (coders[i])
	{
		printf("Coder ID: %lld, Compile Count: %lld\n", coders[i]->id, coders[i]->compile_count);
		if (coders[i]->id == id)
		{
			coders[i]->compile_count++;
			printf("Coder %lld compile count incremented to %lld\n", coders[i]->id, coders[i]->compile_count);
			return ;
		}
		i++;
	}
}
static void print_sim_info(t_simulation *sim)
{
	printf("Simulation Info:\n");
	printf("Number of Coders: %lld\n", sim->args->number_of_coders);
	coder_add(sim->coders, 5);
	printf("Time to Burnout: %lld ms\n", sim->args->time_to_burnout);
	printf("Time to Compile: %lld ms\n", sim->args->time_to_compile);
	printf("Time to Debug: %lld ms\n", sim->args->time_to_debug);
	printf("Time to Refactor: %lld ms\n", sim->args->time_to_refactor);
	printf("Number of Compiles Required: %lld\n", sim->args->number_of_compiles_required);
	printf("Dongle Cooldown: %lld ms\n", sim->args->dongle_cooldown);
	printf("Scheduler: %s\n", sim->args->scheduler);
}

static int data_dispatch(t_args *data)
{
	if (!data)
		return (err_msg(MALLOC_ERROR), 0);
	if (data == (t_args *)0x1)
		return (err_msg(ARG_ERROR), 0);
	if (data == (t_args *)0x2)
		return (err_msg(SCHEDULER_ERROR), 0);
	return (1);
}

static int run_simulation(t_simulation *sim)
{
	pthread_t	*threads;

	if (!sim)
		return (-1);
	sim->start_time = get_current_time();
	pthread_mutex_lock(&sim->state->print_lock);
	printf("\n========== SIMULATION START ==========\n");
	printf("Coders: %lld | Dongles: %lld\n",
		sim->args->number_of_coders, sim->args->number_of_coders);
	printf("Scheduler: %s\n", sim->args->scheduler);
	printf("=====================================\n\n");
	pthread_mutex_unlock(&sim->state->print_lock);
	threads = NULL;
	if (!launch_coder_threads(sim, &threads))
	{
		pthread_mutex_lock(&sim->state->print_lock);
		printf("Error: Failed to launch coder threads\n");
		pthread_mutex_unlock(&sim->state->print_lock);
		return (-1);
	}
	if (!join_coder_threads(sim, threads))
	{
		pthread_mutex_lock(&sim->state->print_lock);
		printf("Error: Failed to join coder threads\n");
		pthread_mutex_unlock(&sim->state->print_lock);
		return (-1);
	}
	pthread_mutex_lock(&sim->state->global_lock);
	sim->state->simulation_running = 0;
	pthread_mutex_unlock(&sim->state->global_lock);
	pthread_mutex_lock(&sim->state->print_lock);
	printf("\n========== SIMULATION END ==========\n");
	printf("Total Compiles: %lld\n", sim->state->total_compiles);
	printf("=====================================\n\n");
	pthread_mutex_unlock(&sim->state->print_lock);
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
	print_sim_info(simulation);
	if (run_simulation(simulation) == -1)
		return (free_simulation(simulation), err_msg(SIM_ERR), -1);
	free_simulation(simulation);
	return (0);
}
