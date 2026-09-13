/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 00:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/13 00:00:00 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

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

static int	node_before(t_heap *h, t_heap_node *a, t_heap_node *b)
{
	if (h->edf_mode && a->key1 != b->key1)
		return (a->key1 < b->key1);
	return (a->key2 < b->key2);
}

static void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_heap *h, long long i)
{
	long long	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!node_before(h, &h->nodes[i], &h->nodes[parent]))
			break ;
		swap_nodes(&h->nodes[i], &h->nodes[parent]);
		i = parent;
	}
}

static void	sift_down(t_heap *h, long long i)
{
	long long	left;
	long long	right;
	long long	smallest;

	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		smallest = i;
		if (left < h->size && node_before(h, &h->nodes[left], &h->nodes[smallest]))
			smallest = left;
		if (right < h->size && node_before(h, &h->nodes[right], &h->nodes[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		swap_nodes(&h->nodes[i], &h->nodes[smallest]);
		i = smallest;
	}
}

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

void	*heap_remove(t_heap *h, void *data)
{
	long long	i;
	void		*found;

	if (!h)
		return (NULL);
	i = 0;
	while (i < h->size && h->nodes[i].data != data)
		i++;
	if (i == h->size)
		return (NULL);
	found = h->nodes[i].data;
	h->size--;
	if (i != h->size)
	{
		h->nodes[i] = h->nodes[h->size];
		sift_up(h, i);
		sift_down(h, i);
	}
	return (found);
}

void	heap_destroy(t_heap *h)
{
	if (!h)
		return ;
	free(h->nodes);
	free(h);
}
