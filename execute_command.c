#include "MyShell.h"

void is_internal_cmd()
{
    if(strcmp(command, "exit") == 0)
        exit(0);
    else if(strcmp(command, "path") == 0)
    {
        char *ch_path = NULL;

        if(parameters[1] == NULL)
        {
            path_flag = false;
            strcpy(gpath[8], "");
            return;
        }
        else if(parameters[1][0] == '~')
        {
            ch_path = malloc(strlen(path) + strlen(parameters[1]));
            if(ch_path == NULL)
                perror("-MyShell error: path: malloc failed.\n");
            strcpy(ch_path, path);
            strncpy(ch_path + strlen(path), parameters[1] + 1, strlen(parameters[1]));
        }
        else if(parameters[1][0] == '.' && parameters[1][1] == '/')
        {
            ch_path = malloc(MAX_PATH_LEN);
            if(ch_path == NULL)
                perror("-MyShell error: path: malloc failed.\n");
            getcwd(ch_path, MAX_PATH_LEN);
            strcpy(ch_path + strlen(ch_path), "/");
        }
        else if(parameters[1][0] == '.' && parameters[1][1] == '.' && parameters[1][2] == '/')
        {
            ch_path = malloc(MAX_PATH_LEN);
            char *ch_path_tmp;
            ch_path_tmp = malloc(MAX_PATH_LEN);
            if(ch_path == NULL || ch_path_tmp == NULL)
                perror("-MyShell error: path: malloc failed.\n");
            getcwd(ch_path, MAX_PATH_LEN);
            int i;
            int index = -1;
            for(i = (int)strlen(ch_path)-1; i >= 0; i--)
            {
                if(ch_path[i] == '/')
                {
                    index = i;
                    break;
                }
            }
            if(index != -1)
                strncpy(ch_path_tmp, ch_path, i+1);
            else
                perror("-MyShell error: path: can not find this path.\n");
            strcpy(ch_path, ch_path_tmp);
            free(ch_path_tmp);
        } 
        else
        {
            ch_path = malloc(strlen(parameters[1] + 1));
            if(ch_path == NULL)
                perror("-MyShell error: path: malloc failed.\n");
            strcpy(ch_path, parameters[1]);
        }
            
        path_flag = true;
        strcpy(gpath[8],ch_path);
        free(ch_path);
    }
    else if(strcmp(command, "about") == 0)
    {
        printf("This is a simple Shell for Linux.\n");
        return;
    }
    else if(strcmp(command, "cd") == 0)
    {
        char *cd_path = NULL;

        if(parameters[1] == NULL)
        {
            parameters[1] = malloc(sizeof(char));
            parameters[1][0] = '~';
        }

        if(parameters[1][0] == '~')
        {
            cd_path = malloc(strlen(path) + strlen(parameters[1]));
            if(cd_path == NULL)
                perror("-MyShell error: cd: malloc failed.\n");
            strcpy(cd_path, path);
            strncpy(cd_path + strlen(path), parameters[1] + 1, strlen(parameters[1]));
        }
        else
        {
            cd_path = malloc(strlen(parameters[1] + 1));
            if(cd_path == NULL)
                perror("-MyShell error: cd: malloc failed.\n");
            strcpy(cd_path, parameters[1]);
        }
         
        if(chdir(cd_path) != 0)
            printf("-MyShell error: cd: %s: %s.\n",cd_path,strerror(errno));
        free(cd_path);
    }
}

void is_pipe()
{
    close(fd[0]);
    close(1);
    dup2(fd[1],1);
    close(fd[1]);
    is_normal_cmd();
}

void is_in_redirect()
{
    int in_fd = open(info.in_file, O_CREAT|O_RDONLY, 0666);
    close(0);
    dup2(in_fd, 0);
    is_normal_cmd();    
    close(in_fd);
}

void is_out_redirect()
{
    int out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_TRUNC, 0666);
    close(1);
    dup2(out_fd, 1);
    is_normal_cmd();     
    close(out_fd);
}

void is_out_redirect_append()
{
    int out_fd = open(info.out_file, O_WRONLY|O_CREAT|O_APPEND, 0666);
    close(1);
    dup2(out_fd, 1);
    is_normal_cmd();     
    close(out_fd);
}

void is_normal_cmd()
{
    if(find_command(command))
    {
        if(path_flag)
        {
            char * cmd_path;
            cmd_path = malloc(MAX_PATH_LEN);
            if(cmd_path == NULL)
                perror("-MyShell error: cmd: malloc failed.\n"); 
            strcpy(cmd_path, gpath[8]);
            strncpy(cmd_path + strlen(gpath[8]), command, strlen(command));
            execv(cmd_path,parameters);
        }            
        else
            execvp(command,parameters);
    }
    else
    {
        perror("-MyShell error");    
        exit(1);
    }  
}

bool find_command(char *cmd)
{
    int i;
    char *cm_path[MAX_SYS_PATH+1];
    for(i = 0; i < MAX_SYS_PATH + 1; i++)
    {
        cm_path[i] = malloc(sizeof(char)*MAX_PATH_LEN);
        if(cm_path[i] == NULL)
            perror("-MyShell error: malloc failed.\n");
    }
    
    if(strncmp(cmd, "/", 1) == 0 || strncmp(cmd, "./", 2) == 0 
        || strncmp(cmd, "../", 3) == 0)
    {
        if(strncmp(cmd, "/", 1) == 0) 
            cmd += 1;
        else if(strncmp(cmd, "./", 2) == 0)
            cmd += 2;
        else if(strncmp(cmd, "../", 3) == 0)
            cmd += 3;
        
        if(access(cmd, X_OK) == 0)        
            return true;
    }

   
    if(path_flag)
    {
        strcpy(cm_path[8], gpath[8]);
        strncpy(cm_path[8] + strlen(gpath[8]), cmd, strlen(cmd));
        if(access(cm_path[8], X_OK) == 0)
            return true;
    }
    else
    {    
        for(i = 0; i < MAX_SYS_PATH; i++)
        {   
            strcpy(cm_path[i], gpath[i]);
            strncpy(cm_path[i] + strlen(cm_path[i]), cmd, strlen(cmd));
            strcpy(cm_path[i] + strlen(cm_path[i]), "\0");
            if(access(cm_path[i], X_OK) == 0)
                return true;
        }     
    }
    
    for(i = 0; i < MAX_SYS_PATH + 1; i++)
        free(cm_path[i]);
    
    return false;
}

