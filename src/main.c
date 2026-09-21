/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 15:50:05 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/21 20:49:22 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/codexion.h"

int	main(int ac, char **av)
{
	if (ac != 9)
		return (err_msg(N_ARG_ERROR), 0);
	else
		return (main_loop(av));
}
