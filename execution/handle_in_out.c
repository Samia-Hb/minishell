#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../minishell.h"

int rfd_in(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = -1;
    while (n)
    {
        if (n->type == REDERECTION_IN)
        {
            if (fd > 0) close(fd);
            // fd = open(n->data->arguments[1], O_RDONLY);
            if (fd < 0)
            {
                perror("Error opening input file");
                return -1;
            }
        }
        n = n->right;
    }
    cmd->data->input_fd = fd;
    return fd;  
}

int rfd_out(t_ast *cmd)
{
    t_ast *n = cmd->left;
    int fd = -1;
    while (n)
    {
        if (n->type == REDERECTION_OUT)
        {
            if (fd > 0) close(fd);
            // fd = open(n->data->arguments[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Error opening output file");
                return -1;
            }
        }
        n = n->right;
    }
    cmd->data->output_fd = fd;
    return fd;  
}

void exec_command_with_redirection(t_ast *cmd)
{
    int in_fd = rfd_in(cmd);
    int out_fd = cmd->data->output_fd;
    pid_t pid = fork(); 
    if (pid < 0)
    {
        perror("Fork failed");
        return;
    }
    if (pid == 0)
    {  
        // if (in_fd >= 0)
        // {
        //     int save_out = dup(STDOUT_FILENO);
        //     if(dup2(save_out, STDOUT_FILENO) < 0)
        //     {
        //         printf("ghalat hdshi\n");
        //         return;
        //     }
        //     dup2(save_out, STDIN_FILENO);
        //     close(in_
        // }
        if (out_fd >= 0)
        {
        // printf("child proccess\n");
        // exit(1);
        //     printf("hda red_out\n");
        //     exit(1);
            // int saved_out = dup(STDOUT_FILENO);
            // if(dup2(saved_out, STDOUT_FILENO) )
         {
        // printf("heeey there\n");
        // // exit(1);
        // rfd_out(cmd);
        // int saved_stdou = dup(STDOUT_FILENO);
        printf("command\n");
        exit(1);
        if(dup2(cmd->data->output_fd, STDOUT_FILENO) < 0)
        {
            printf("ERROR");
            return ;
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
            // if (dup2(out_fd, STDOUT_FILENO) < 0)
            // {
            //     printf("error\n");
            //     // perror("Failed to redirect output");
            //     _exit(1);
            // }
            // printf("sucess\n");
            // exit(1);
            // // dup2(out_fd, STDOUT_FILENO);
            // dup2(out_fd, STDOUT_FILENO);
            // close (out_fd);
        // execvp(cmd->data->arguments[0], cmd->data->arguments);
        // perror("Execution failed");
        // _exit(1); 
        }
    } 
    else
    {
        waitpid(pid, NULL, 0);
        if (in_fd >= 0) close(in_fd);
        if (out_fd >= 0) close(out_fd);
    }
}


