/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/19 17:31:42 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"
#include <stdlib.h>

t_queue	*queue_create(void)
{
	t_queue	*q;

	q = (t_queue *)malloc(sizeof(t_queue));
	if (!q)
		return (NULL);
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	if (pthread_mutex_init(&q->mutex, NULL) != 0)
	{
		free(q);
		return (NULL);
	}
	return (q);
}

void	queue_push(t_queue *q, void *data)
{
	t_queue_node	*new_node;

	if (!q)
		return ;
	new_node = (t_queue_node *)malloc(sizeof(t_queue_node));
	if (!new_node)
		return ;
	new_node->data = data;
	new_node->next = NULL;
	pthread_mutex_lock(&q->mutex);
	if (q->tail)
		q->tail->next = new_node;
	else
		q->head = new_node;
	q->tail = new_node;
	q->size++;
	pthread_mutex_unlock(&q->mutex);
}

void	*queue_pop(t_queue *q)
{
	void			*data;
	t_queue_node	*temp;

	if (!q)
		return (NULL);
	pthread_mutex_lock(&q->mutex);
	if (!q->head)
	{
		pthread_mutex_unlock(&q->mutex);
		return (NULL);
	}
	temp = q->head;
	data = q->head->data;
	q->head = q->head->next;
	if (!q->head)
		q->tail = NULL;
	q->size--;
	pthread_mutex_unlock(&q->mutex);
	free(temp);
	return (data);
}

void	*queue_peek(t_queue *q)
{
	void	*data;

	if (!q)
		return (NULL);
	pthread_mutex_lock(&q->mutex);
	data = NULL;
	if (q->head)
		data = q->head->data;
	pthread_mutex_unlock(&q->mutex);
	return (data);
}

int	queue_remove(t_queue *q, void *data)
{
	t_queue_node	*cur;
	t_queue_node	*prev;

	if (!q)
		return (0);
	pthread_mutex_lock(&q->mutex);
	cur = q->head;
	prev = NULL;
	while (cur)
	{
		if (cur->data == data)
		{
			if (prev)
				prev->next = cur->next;
			else
				q->head = cur->next;
			if (cur == q->tail)
				q->tail = prev;
			q->size--;
			pthread_mutex_unlock(&q->mutex);
			free(cur);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	pthread_mutex_unlock(&q->mutex);
	return (0);
}
