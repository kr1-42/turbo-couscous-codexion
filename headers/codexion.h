#ifndef CODEXION_H
# define CODEXION_H

#include <unistd.h>

#define N_ARG_ERROR "invalid number of arguments ... usage\n\n./codexion number_of_coders " \
"time_to_burnout time_to_compile time_to_debug time_to_refactor number_of" \
"_compiles_required dongle_cooldown scheduler"

#define MALLOC_ERROR "malloc error accured\n"

void	err_msg(char *msg);


// utils

int		    ft_strlen(char *str);
long long	ft_atoll(const char *number_str);


#endif
