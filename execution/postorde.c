#include "../minishell.h"

void algo_execution(t_ast *cmd, t_mini *box)
{
    if (!cmd)
        return;
    if (cmd->type == COMMAND)
    {
        if (rfd_in(cmd) < 0)
            return;
        if (rfd_out(cmd) < 0)
            return;
        executing(cmd, box);
    }
    else if (cmd->type == PIPELINE)
        execute_pipeline(cmd, box);
    algo_execution(cmd->left, box);
    algo_execution(cmd->right, box);
}

