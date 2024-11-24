#include "../../../minishell.h"

extern char **environ;

char **separate_env(t_envi *env)
{
    int count = 0;
    t_envi *tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **the_env = malloc(sizeof(char *) * (count + 1));
    if (!the_env)
    {
        perror("Error allocating memory");
        return NULL;
    }
    int i = 0;
    while (env)
    {
        size_t name_len = strlen(env->name);
        size_t value_len = strlen(env->vale);
        the_env[i] = malloc(name_len + value_len + 2);
        if (!the_env[i])
        {
            perror("Error allocating memory for environment variable string");
            return NULL;
        }
        strcpy(the_env[i], env->name);
        the_env[i][name_len] = '=';
        strcpy(the_env[i] + name_len + 1, env->vale);
        env = env->next;
        i++;
    }
    the_env[i] = NULL;
    return the_env;
}

// <<<<<<< HEAD

// void	execs(t_cmd *cmd, int btn, t_mini *box)
// {
//     char **env_array;

//     if (btn != -1)
//     {
//         exec_builtin(btn, cmd, box);
//         exit(0);
//     }
//     if (cmd->cmd_path)
//     {
//         env_array = separate_env(box->env); 
//         if (!env_array)
//         {
//             perror("Failed to convert environment variables");
//             exit(1);
//         }
//         execve(cmd->cmd_path, cmd->arguments, env_array);
//         perror(cmd->cmd_path);
//         free(env_array); 
//         exit(errno);
//     }
// }
// =======
void	execs(t_cmd *token, int btn, t_mini *env)
{
	if (btn != -1)
	{
		exec_builtin(btn, token, env);
		exit(0);
	}
	if (token->cmd_path)
	{
	g_var->en = separate_env(env->env);
		if (execve(token->cmd_path, token->arguments, g_var->en) == -1)
		{
		printf("ls\n");
		exit(1);
			ft_putstr_fd("minishell: ", 2);
			// ft_putstr_fd(strerror(1), 2);
			ft_putstr_fd("\n", 2);
			// free_hdfiles();
			exit(1);
		}
	}
	else
		exit(0);
}

int	init_execute_arguments(void)
{
	// g_var->exit_status = 0;
	g_var->pre_pipe_infd = -1;
	return (0);
}

void	cleanup_execute_arguments(t_cmd *token)
{
	if (g_var->pre_pipe_infd > 2)
		close(g_var->pre_pipe_infd);
	sig_wait(token);
}


// void	exec_builtin(int btn, t_cmd *cmd, t_mini *box)
// {
// 	if (btn == 1)
// 		ft_cd(cmd->arguments, box->env);
// 	else if (btn == 2)
// 		ft_echo(cmd->arguments);
// 	else if (btn == 3)
// 		ft_env(box->env);
// 	else if (btn == 4)
// 		ft_exit(cmd->arguments);
// 	else if (btn == 5)
// 		ft_export(cmd->arguments, &box->env);
// 	else if (btn == 6)
// 		ft_pwd(cmd->arguments, box->env);
// 	else if (btn == 7)
// 		ft_unset(cmd->arguments, box);
// 	if (g_var->out_fd > 2)
// 		close(g_var->out_fd);
// 	g_var->out_fd = 1;
// }
void	execute_arguments(t_cmd *token, t_mini *env)
{
	int			i;
	t_cmd	*current;

	if (!token)
		return ;
	init_execute_arguments();
	i = 0;
	current = token;
	while (current && !g_var->exit_status)
	{
		execute_pipes(current, i, env);
		current = current->next;
		i++;
	}
	g_var->num = i;
	cleanup_execute_arguments(token);
}
