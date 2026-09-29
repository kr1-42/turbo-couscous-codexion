/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exitscreen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrilomb <chrilomb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 14:55:43 by chrilomb          #+#    #+#             */
/*   Updated: 2026/09/29 16:34:59 by chrilomb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/codexion.h"

static void	*input_listener(void *arg)
{
	t_exbuf	*b;
	char	c;

	b = (t_exbuf *)arg;
	while (read(0, &c, 1) > 0 && (c == 0x1B || c == 0x0A))
		;
	pthread_mutex_lock(&b->lock);
	b->stop = 1;
	pthread_mutex_unlock(&b->lock);
	return (NULL);
}

void	ex_add(t_exbuf *b, const char *s)
{
	int	n;

	while (*s)
	{
		n = 0;
		while (*s >= '0' && *s <= '9')
			n = n * 10 + (*s++ - '0');
		if (n == 0)
			n = 1;
		while (n-- > 0)
		{
			if (*s == '|')
				b->data[b->len++] = '\n';
			else
				b->data[b->len++] = *s;
		}
		s++;
	}
}

static int	load_frames(t_exbuf *b)
{
	b->len = 0;
	b->data = malloc(EX_FRAME_SIZE * EX_NB_FRAMES);
	if (!b->data)
		return (1);
	ex_data_0(b);
	ex_data_1(b);
	ex_data_2(b);
	ex_data_3(b);
	ex_data_4(b);
	ex_data_5(b);
	ex_data_6(b);
	ex_data_7(b);
	ex_data_8(b);
	ex_data_9(b);
	ex_data_10(b);
	ex_data_11(b);
	ex_data_12(b);
	ex_data_13(b);
	ex_data_14(b);
	ex_data_15(b);
	ex_data_16(b);
	ex_data_17(b);
	return (0);
}

static void	play(t_exbuf *b)
{
	int	f;
	int	dir;
	int	stop;

	f = 0;
	dir = 1;
	stop = 0;
	while (!stop)
	{
		write(1, "\033[2J\033[H", 7);
		write(1, b->data + f * EX_FRAME_SIZE, EX_FRAME_SIZE);
		usleep(EX_DELAY_US);
		f += dir;
		if (f >= EX_NB_FRAMES - 1)
			dir = -1;
		if (f <= 0)
			dir = 1;
		pthread_mutex_lock(&b->lock);
		stop = b->stop;
		pthread_mutex_unlock(&b->lock);
	}
}

int	exit_screen(void)
{
	t_exbuf		b;
	pthread_t	thread;

	if (load_frames(&b))
		return (1);
	b.stop = 0;
	if (pthread_mutex_init(&b.lock, NULL))
		return (free(b.data), 1);
	if (pthread_create(&thread, NULL, input_listener, &b))
	{
		pthread_mutex_destroy(&b.lock);
		return (free(b.data), 1);
	}
	play(&b);
	pthread_join(thread, NULL);
	pthread_mutex_destroy(&b.lock);
	return (free(b.data), 0);
}
