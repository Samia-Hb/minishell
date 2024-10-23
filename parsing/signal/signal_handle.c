#include "../../minishell.h"

void	handle_ctrl_c(void)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_ctrl_d(void)
{
	exit(127);
}

void	handle_signal(void)
{
	signal(SIGINT, (__sighandler_t)handle_ctrl_c);
	signal(SIGTERM,  (__sighandler_t)handle_ctrl_d);
}
