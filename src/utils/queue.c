/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 14:00:00 by chrilomb          #+#    #+#             */
/*   Updated: 2026/05/19 14:05:46 by chrilomb         ###   ########.fr       */
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

void	queue_destroy(t_queue *q)
{
	t_queue_node	*temp;

	if (!q)
		return ;
	pthread_mutex_lock(&q->mutex);
	while (q->head)
	{
		temp = q->head;
		q->head = q->head->next;
		free(temp);
	}
	pthread_mutex_unlock(&q->mutex);
	pthread_mutex_destroy(&q->mutex);
	free(q);
}
