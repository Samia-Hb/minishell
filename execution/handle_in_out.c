#include"../minishell.h"

int rfd_in(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = 0;
    while (n)
    {
        if (n->type == REDERECTION_IN)
        {
            if (fd > 0)
                close(fd);
            fd = open(n->data->token->value, O_RDONLY);
            if (fd < 0)
                return -1;
            if (dup2(fd, STDIN_FILENO) < 0)
                return -1;
        }
        n = n->right;
    }
    cmd->data->input_fd = fd;
    return 0;
}

int rfd_out(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = 0;
    while (n)
    {
        if (n->type == REDERECTION_OUT)
        {
            if (fd > 0)
                close(fd);
            fd = open(n->data->token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
                return -1;
            if (dup2(fd, STDOUT_FILENO) < 0)
                return -1;
        }
        else if (n->type == REDERECTION_APPEND)
        {
            if (fd > 0)
                close(fd);
            fd = open(n->data->token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
                return -1;
            if (dup2(fd, STDOUT_FILENO) < 0)
                return -1;
        }
        n = n->right;
    }
    cmd->data->output_fd = fd;
    return 0;
}

void exec_cmd(t_ast *cmd, t_mini *box)
{
    pid_t pid = fork();
    if (pid < 0)
        return;
    if (pid == 0)
    {
        algo_execution(cmd, box);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}
