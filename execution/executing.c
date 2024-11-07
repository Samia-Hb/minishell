#include "../minishell.h"

void handle_redirections(t_cmd *cmd)
{
    t_file *file = cmd->file;
    while (file)
    {
        int fd;
        if (file->type == RE_IN)
            fd = open(file->filename, O_RDONLY);
        else if (file->type == RE_OUT)
            fd = open(file->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (file->type == RE_APPEND)
            fd = open(file->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            file = file->next;
        if (fd < 0)
        {
            perror("Error opening file ");
            exit(EXIT_FAILURE);
        }
        if (file->type == RE_IN)
            dup2(fd, STDIN_FILENO);
        else
            dup2(fd, STDOUT_FILENO);

        close(fd);
        file = file->next;
    }
}

int count_env_vars(t_envi *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

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
        size_t name_len = ft_strlen(env->name);
        size_t value_len = ft_strlen(env->vale);
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

void execute_single_builtin(t_cmd *cmd, t_mini *box, int builtin_index)
{
    handle_redirections(cmd);  
    int status = exec_builtin(builtin_index, cmd->arguments, box);
    exit(status); 
}

void execute_pipeline(t_cmd *cmd_list, t_mini *box)
{
    int pipe_fd[2], in_fd = STDIN_FILENO;
    t_cmd *current = cmd_list;

    while (current)
    {
        if (current->next)
        {
            if (pipe(pipe_fd) < 0)
            {
                perror("Pipe error");
                exit(EXIT_FAILURE);
            }
        }
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            if (in_fd != STDIN_FILENO)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (current->next)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            handle_redirections(current);
            execute_command(current, box);
            exit(EXIT_SUCCESS); 
        }
        close(in_fd);
        if (current->next)
        {
            close(pipe_fd[1]);
            in_fd = pipe_fd[0];
        }
        current = current->next;
    }
    int status;
    while (wait(&status) > 0)
    {
        if (WIFEXITED(status))
            box->last_exit_status = WEXITSTATUS(status);
    }
}

int check_builtin(char *cmd)
{
    if(strcmp(cmd, "cd") == 0)
        return 0;
    else if(strcmp(cmd, "echo") == 0)
        return 1;
    else if(strcmp(cmd, "pwd") == 0)
        return 2;
    else if(strcmp(cmd, "exit") == 0)
        return 3;
    else if(strcmp(cmd, "export") == 0)
        return 4;
    else if(strcmp(cmd, "env") == 0)
        return 5;
    else if(strcmp(cmd, "unset") == 0)
        return 6;
    return -1;
}

char *get_cmd_path(char *cmd, t_envi *env)
{
    char *path_env = NULL;
    t_envi *current = env;

    while (current)
    {
        if (strcmp(current->name, "PATH") == 0)
        {
            path_env = current->vale;
            break;
        }
        current = current->next;
    }

    if (!path_env)
        return NULL;

    char **paths = ft_split(path_env, ':');
    if (!paths)
        return NULL;

    char *full_path = NULL;
    int i = 0;
    while(paths[i])
    {
        full_path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
        if (!full_path)
        {
            for (int j = 0; paths[j]; j++)
                free(paths[j]);
            free(paths);
            return NULL;
        }
        strcpy(full_path, paths[i]);
        strcat(full_path, "/");
        strcat(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            for (int j = 0; paths[j]; j++)
                free(paths[j]);
            free(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }
    int j = 0;
    while(paths[j])
    {
        free(paths[j]);
        j++;
    }
    free(paths);
    return NULL;
}

int exec_builtin(int builtin_index, char **arguments, t_mini *box)
{
    if (builtin_index == 0)
        return (ft_cd(arguments, box->env));
    else if (builtin_index == 1)
        return(ft_echo(arguments));
    else if (builtin_index == 2)
        return(ft_pwd(box->env));
    else if (builtin_index == 3)
        return(ft_exit(box->shell));
    else if (builtin_index == 4)
        return(ft_export(arguments, box->env));
    else if (builtin_index == 5)
        return(ft_env(box->env));
    else if (builtin_index == 6)
        return(ft_unset(arguments, box));
    return -1;
}

void execute_command(t_cmd *cmd, t_mini *box)
{
    int builtin_index = check_builtin(cmd->arguments[0]);
        pid_t pid = fork();
    if (builtin_index != -1)
    {
        if (pid == 0)
        {
            execute_single_builtin(cmd, box, builtin_index);
            exit(EXIT_SUCCESS);
        }
        else
        {
            int status;
            wait(&status);
            box->last_exit_status = WEXITSTATUS(status);
        }
    }
    else
    {
        char *path = get_cmd_path(cmd->arguments[0], box->env);
        if (!path)
        {
           perror("error");
            return;
        }
        if (pid == 0)
        {
            char **env_array = separate_env(box->env);
            if (!env_array)
            {
                perror("Error converting environment variables");
                exit(EXIT_FAILURE);
            }
            execve(path, cmd->arguments, env_array);
            perror("execve error");
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            wait(&status);
            box->last_exit_status = WEXITSTATUS(status);
        }
    }
}

int count_pipes(t_cmd *cmd_list)
{
    int count = 0;
    t_cmd *current = cmd_list;

    while (current)
    {
        if (current->type == PIPE)
            count++;
        current = current->next;
    }
    return count;
}

