/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 15:32:32 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/30 15:46:11 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

static void	ft_putchar(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr(const char *s, int fd)
{
	while (*s)
	{
		ft_putchar(*s, fd);
		s++;
	}
}

void	ft_error_msg(int ctx, int err, int fd)
{
    if (ctx == 1)
    {
        else if (err == 3)
            ft_putstr(ARG_ERROR, fd);
        else if (err == 4)
            ft_putstr(MALLOC_ERROR, fd);
        else if (err == 5)
            ft_putstr(SIM_ERR, fd);
    }
}


