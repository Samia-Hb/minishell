#include "../minishell.h"


void executing(t_ast *node, t_mini *box)
{
    if (node->type == COMMAND)
    {
        if (!node->data->arguments)
            return;
        if (is_builtin(node->data->token->value))
        {
            builtins(node->data->arguments, box);
            return; 
        }
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Fork failed");
            return;
        }
        if (pid == 0)
        {
            char **command_path = get_path();
            if (!command_path)
            {
                free(node->data->arguments);
                exit(EXIT_FAILURE);
            }
            char **env_array = separate_env(box->env);
            if (!env_array)
            {
                free(node->data->arguments);
                free(command_path);
                exit(EXIT_FAILURE);
            }
            if (node->data->input_fd != STDIN_FILENO) 
            {
                dup2(node->data->input_fd, STDIN_FILENO);
                close(node->data->input_fd);
            }
            if (node->data->output_fd != STDOUT_FILENO)
            {
                dup2(node->data->output_fd, STDOUT_FILENO);
                close(node->data->output_fd);
            }
            char *command = node->data->arguments[0];
            if (access(command, X_OK) == 0) 
                execve(command, node->data->arguments, env_array);
            else 
            {
                int i = 0;
                char *full_path = NULL;
                while (command_path[i])
                {
                    char *temp = ft_strjoin(command_path[i], "/");
                    if (!temp) 
                        exit(EXIT_FAILURE);
                    full_path = ft_strjoin(temp, command);
                    free(temp);
                    if (access(full_path, X_OK) == 0)
                    {
                        execve(full_path, node->data->arguments, env_array);
                        perror("execve error");
                        exit(EXIT_FAILURE); 
                    }
                    free(full_path);
                    i++;
                }
                perror("command not found");
            }
            free(node->data->arguments);
            int j = 0;
            while (command_path[j])
            {
                free(command_path[j]);
                j++;
            }
            free(command_path);
            j = 0;
            while (env_array[j])
            {
                free(env_array[j]);
                j++;
            }
            free(env_array);
            exit(EXIT_FAILURE); 
        } 
        else 
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}
