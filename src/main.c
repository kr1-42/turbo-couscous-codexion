#include "../headers/codexion.h"

int main(int ac, char **av)
{
	if (ac != 9)
		return (err_msg(N_ARG_ERROR), 0);
	else
		return (main_loop(av));
}
