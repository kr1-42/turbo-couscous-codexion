/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   q_too.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 17:32:03 by chrilomb          #+#    #+#             */
/*   Updated: 2026/07/19 17:32:18 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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
