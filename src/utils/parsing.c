/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"
#define MAX_ARG 2147483647LL

static long long	is_valid_number(char *str)
{
	int			i;
	long long	value;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	if (i == 0 || i > 10)
		return (-1);
	value = ft_atoll(str);
	if (value < 0 || value > MAX_ARG)
		return (-1);
	return (value);
}

static int	fill_numbers_struct(t_args *ptr, char **av)
{
	ptr->number_of_coders = is_valid_number(av[1]);
	if (ptr->number_of_coders < 1)
		return (-1);
	ptr->time_to_burnout = is_valid_number(av[2]);
	if (ptr->time_to_burnout == -1)
		return (-1);
	ptr->time_to_compile = is_valid_number(av[3]);
	if (ptr->time_to_compile == -1)
		return (-1);
	ptr->time_to_debug = is_valid_number(av[4]);
	if (ptr->time_to_debug == -1)
		return (-1);
	ptr->time_to_refactor = is_valid_number(av[5]);
	if (ptr->time_to_refactor == -1)
		return (-1);
	ptr->number_of_compiles_required = is_valid_number(av[6]);
	if (ptr->number_of_compiles_required == -1)
		return (-1);
	ptr->dongle_cooldown = is_valid_number(av[7]);
	if (ptr->dongle_cooldown == -1)
		return (-1);
	ptr->scheduler = av[8];
	return (0);
}

t_args	*parse_data(char **av)
{
	t_args	*ptr;

	ptr = (t_args *)malloc(sizeof(t_args));
	if (!ptr)
		return (NULL);
	if (fill_numbers_struct(ptr, av) == -1)
		return (free(ptr), (t_args *)1);
	if (ft_strcmp(ptr->scheduler, "edf") != 0
		&& ft_strcmp(ptr->scheduler, "fifo") != 0)
		return (free(ptr), (t_args *)2);
	ptr->edf_mode = (ft_strcmp(ptr->scheduler, "edf") == 0);
	return (ptr);
}
