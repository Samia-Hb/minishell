#include "../minishell.h"

void algo_execution(t_ast *cmd, t_mini *box)
{
    if(!cmd)
        return;
    if(cmd->type == COMMAND)
        executing(cmd, box);
    else if(cmd->type == PIPELINE)
        execute_pipeline(cmd, box);
    else if(cmd->type == REDERECTION_IN)
        redir_fd_in(cmd);
    else if(cmd->type == REDERECTION_OUT)
        redir_fd_out(cmd);
    algo_execution(cmd->left, box);
    algo_execution(cmd->right, box);
}

//nothing