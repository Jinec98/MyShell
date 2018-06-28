#include "MyShell.h"

void parse_info_init(struct parse_info * info)
{
    info->flag = 0;
    info->run_flag = false;
    info->in_file = NULL;
    info->out_file = NULL;
    info->command2 = NULL;
    info->parameters2 = NULL;
}

void parsing_command(struct parse_info * info)
{
    parse_info_init(info);
    
    if(strcmp(command, "exit") == 0 || strcmp(command, "path") == 0 ||
       strcmp(command, "about") == 0 || strcmp(command, "cd") == 0)
    {
        info->flag = IS_INTERNAL_CMD;
        return;
    }

    if(paraNum > 0 && strcmp(parameters[paraNum-1], "&") == 0)
    {
        info->run_flag = IS_BACKGROUND;
        parameters[paraNum-1] = NULL;
        paraNum--;
    }
    
    int i;
    for(i = 0; i < paraNum; i++)
    {
        if(strcmp(parameters[i], "<") == 0 || strcmp(parameters[i], "<<") == 0)
        {
            info->flag = IS_IN_REDIRECT;
            info->in_file = parameters[i+1];
            parameters[i] = NULL;
            break;
        }
        else if(strcmp(parameters[i], ">") == 0)
        {
            info->flag = IS_OUT_REDIRECT;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            break;
        }
        else if(strcmp(parameters[i], ">>") == 0)
        {
            info->flag = IS_OUT_REDIRECT_APPEND;
            info->out_file = parameters[i+1];
            parameters[i] = NULL;
            break;
        }
        else if(strcmp(parameters[i], "|") == 0)
        {
            info->flag = IS_PIPE;
            parameters[i] = NULL;
            info->command2 = parameters[i+1];
            info->parameters2 = &parameters[i+1];
            break;
        }
    }
    if(i == paraNum)
        info->flag = IS_NORMAL_CMD;
}
