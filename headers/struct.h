#ifndef STRUCT_H
#define STRUCT_H

umber_of_coders " \
"time_to_burnout time_to_compile time_to_debug time_to_refactor number_of" \
"_compiles_required dongle_cooldown scheduler

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
}
