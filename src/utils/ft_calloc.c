/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrlomba <chrlomba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 20:37:30 by chrlomba          #+#    #+#             */
/*   Updated: 2026/09/21 20:37:34 by chrlomba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

static void	*ft_memset(void *src, int charset, size_t n_bytes)
{
	unsigned char	*src_holder;

	src_holder = (unsigned char *)src;
	while (n_bytes--)
		*src_holder++ = (unsigned char )charset;
	return (src);
}

static void	ft_bzero(void *str, size_t n_bytes)
{
	ft_memset(str, '\0', n_bytes);
}

void	*ft_calloc(size_t num_elems, size_t byte_size)
{
	char	*alloc_ptr;

	alloc_ptr = malloc(num_elems * byte_size);
	if (!alloc_ptr)
		return (NULL);
	else
		ft_bzero(alloc_ptr, num_elems * byte_size);
	return (alloc_ptr);
}
