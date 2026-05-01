#include "../headers/codexion.h"

void	err_msg(char *msg)
{
	write(2, msg, ft_strlen(msg));
}
