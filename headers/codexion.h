#ifndef CODEXION_H
# define CODEXION_H

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "struct.h"

#define N_ARG_ERROR "invalid number of arguments ... usage\n./codexion \n1:number_of_coders \t" \
"2:time_to_burnout \n3:time_to_compile \t4:time_to_debug\n 5:time_to_refactor \t6:number_of" \
"_compiles_required \n7:dongle_cooldown \t8:scheduler\n"

#define SCHEDULER_ERROR "invalid scheduler ... usage\n./codexion ARGS fifo or edf (ALL IN LOWERCASE)\n"

#define ARG_ERROR "only valid positive integers are accepted as arguments\n"

#define MALLOC_ERROR "malloc error accured\n"

#define SIM_ERR "simulation error accured\n"

void	err_msg(char *msg);


// utils
long long   get_current_time(void);
int		    ft_strlen(char *str);
long long	ft_atoll(const char *number_str);
int         ft_strcmp(const char *s1, const char *s2);
void        cleanup_coders(t_coder **coders);
t_simulation    *fill_simulation(t_args *data);
t_args	    *parse_data(char **av);
void       cleanup_dongles(t_dongle **dongles);

// queue operations
t_queue     *queue_create(void);
void        queue_push(t_queue *q, void *data);
void        *queue_pop(t_queue *q);
void        queue_destroy(t_queue *q);
void	*queue_peek(t_queue *q);
int	queue_remove(t_queue *q, void *data);


// simulation
int         main_loop(char **av);
t_simulation *init_simulation(t_args *data);
void        free_simulation(t_simulation *sim);

// actions
int			acquire_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle);
void		release_dongle(t_simulation *sim, t_coder *coder, t_dongle *dongle);
void		action_compile(t_simulation *sim, t_coder *coder, t_dongle *dongle);
void		action_debug(t_simulation *sim, t_coder *coder);
void		action_refactor(t_simulation *sim, t_coder *coder);
int			action_check_burnout(t_simulation *sim, t_coder *coder);
void		action_sleep(long long milliseconds);

// routines
void		*coder_routine(void *arg);
int			launch_coder_threads(t_simulation *sim, pthread_t **threads);
int			join_coder_threads(t_simulation *sim, pthread_t *threads);


#endif
