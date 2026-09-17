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

void	sift_up(t_heap *h, long long i)
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
		if (left < h->size
			&& node_before(h, &h->nodes[left], &h->nodes[smallest]))
			smallest = left;
		if (right < h->size
			&& node_before(h, &h->nodes[right], &h->nodes[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		swap_nodes(&h->nodes[i], &h->nodes[smallest]);
		i = smallest;
	}
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
