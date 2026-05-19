/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_time.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:53:47 by chrilomb          #+#    #+#             */
/*   Updated: 2026/05/19 14:05:59 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

long long   get_current_time(void)
{
    struct timeval  tv;
    long long       milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
    return (milliseconds);
}
