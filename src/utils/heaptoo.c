/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heaptoo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 12:45:47 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/16 12:46:03 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

void	heap_push(t_heap *h, void *data, long long key1, long long key2)
{
	if (!h || h->size >= h->capacity)
		return ;
	h->nodes[h->size].data = data;
	h->nodes[h->size].key1 = key1;
	h->nodes[h->size].key2 = key2;
	h->size++;
	sift_up(h, h->size - 1);
}

void	heap_destroy(t_heap *h)
{
	if (!h)
		return ;
	free(h->nodes);
	free(h);
}
/*
** Array-based binary min-heap. In edf_mode, nodes are ordered by key1
** (deadline) then key2 (arrival sequence) as a tie-breaker. Otherwise,
** nodes are ordered by key2 (arrival sequence) alone, giving FIFO order.
*/

t_heap	*heap_create(long long capacity, int edf_mode)
{
	t_heap	*h;

	h = (t_heap *)malloc(sizeof(t_heap));
	if (!h)
		return (NULL);
	if (capacity < 1)
		capacity = 1;
	h->nodes = (t_heap_node *)malloc(sizeof(t_heap_node) * capacity);
	if (!h->nodes)
		return (free(h), NULL);
	h->size = 0;
	h->capacity = capacity;
	h->edf_mode = edf_mode;
	return (h);
}
