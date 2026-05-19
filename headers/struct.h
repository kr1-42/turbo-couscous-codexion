#ifndef STRUCT_H
#define STRUCT_H

#include <pthread.h>

typedef struct s_simulation t_simulation;

/* ==================== QUEUE NODE & QUEUE ==================== */


typedef struct s_queue_node
{
    void                    *data;
    struct s_queue_node     *next;
}                           t_queue_node;

typedef struct s_queue
{
    t_queue_node    *head;
    t_queue_node    *tail;
    long long       size;
    pthread_mutex_t mutex;
}                   t_queue;

/* ==================== CODER STRUCTURE ==================== */

typedef struct s_coder
{
    long long       id;
    long long       compile_count;
    long long       last_compile_time;
    long long       burnout_time;
    int             is_burned_out;
    pthread_mutex_t mutex;  /* Protects coder's individual state */
}                   t_coder;

/* ==================== DONGLE STRUCTURE ==================== */

typedef struct s_dongle
{
    pthread_mutex_t mutex;      /* Protects dongle state */
    long long       id;
    long long       cooldown_time;
    int             is_available;
}                   t_dongle;

/* ==================== SIMULATION ARGS ==================== */

typedef struct s_args
{
    long long   number_of_coders;
    long long   time_to_burnout;
    long long   time_to_compile;
    long long   time_to_debug;
    long long   time_to_refactor;
    long long   number_of_compiles_required;
    long long   dongle_cooldown;
    char        *scheduler;
}               t_args;

/* ==================== SIMULATION STATE MANAGER ==================== */

typedef struct s_sim_state
{
    pthread_mutex_t global_lock;    /* Global simulation state lock */
	pthread_mutex_t print_lock;     /* Synchronize output printing */
	long long       simulation_running;
	long long       total_compiles;
}                   t_sim_state;

/* ==================== THREAD CONTEXT ==================== */

typedef struct s_thread_context
{
    t_simulation	*sim;
	t_coder			*coder;
}					t_thread_context;

/* ==================== MAIN SIMULATION STRUCTURE ==================== */

typedef struct s_simulation
{
    t_args          *args;
    t_coder         **coders;
    t_dongle        **dongles;
    t_sim_state     *state;
    t_queue         *job_queue;     /* Queue of coders waiting to compile */
    long long       start_time;
}                   t_simulation;
#endif

