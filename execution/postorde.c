#include "../minishell.h"

void algo_execution(t_ast *cmd, t_mini *box)
{
    if (!cmd)
        return;
    printf("cmd->value = %s\n", cmd->data->token->value);
    exit(1);
    if (cmd->type == COMMAND)
    {
        printf("check\n");
        exit(1);
        if (rfd_in(cmd) < 0)
            return;
        if (rfd_out(cmd) < 0)
            return;
        executing(cmd, box);
    }
    else if (cmd->type == PIPELINE)
        execute_pipeline(cmd, box);
    else if (cmd->type == REDERECTION_OUT)
    {
        printf("heeey there\n");
    }
    algo_execution(cmd->right, box);
    algo_execution(cmd->left, box);
}

