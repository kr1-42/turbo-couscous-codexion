#include "../headers/codexion.h"

int	main_loop(char **av)
{
	t_args *data = parse_data(av);
	if (!data)
		return (err_msg(MALLOC_ERROR), -1)
}
