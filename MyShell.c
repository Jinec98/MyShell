#include "MyShell.h"

void init()
{
    parameters = malloc(sizeof(char*)*(MAXARG+1));
    if(parameters == NULL)
        perror("-MyShell error: malloc failed.\n");
    
    path = getenv("HOME");
    int i;
    for(i = 0; i < MAX_SYS_PATH + 1; i++)
    {
        gpath[i] = malloc(sizeof(char)*MAX_PATH_LEN);
        if(gpath[i] == NULL)
            perror("-MyShell error: malloc failed.\n");
    }
    strcpy(gpath[0], "/usr/local/sbin/");
    strcpy(gpath[1], "/usr/local/bin/");
    strcpy(gpath[2], "/usr/sbin/");
    strcpy(gpath[3], "/usr/bin/");
    strcpy(gpath[4], "/sbin/");
    strcpy(gpath[5], "/bin/");
    strcpy(gpath[6], "/usr/games/");
    strcpy(gpath[7], "/usr/local/games/");
    path_flag = false;

    signal(SIGCHLD,SIG_IGN);
}

int main(int argc, char *argv[])
{
    init();

    while(true)
    {
        type_prompt();
        pipe(fd);
        paraNum = read_command();
        if(paraNum == -1)
            continue;
        
        parsing_command(&info);
        if(info.flag == IS_INTERNAL_CMD)
        {
            is_internal_cmd();
            continue;
        }
        
        pid = fork();    
        if(pid < 0)
        {
            perror("-MyShell error: fork failed.\n");
            continue;
        }
        else if(pid == 0)
        {
            switch(info.flag)
            {
                case IS_PIPE:
                    is_pipe();
                    break;
                case IS_IN_REDIRECT:
                    is_in_redirect();
                    break;
                case IS_OUT_REDIRECT:
                    is_out_redirect();
                    break;
                case IS_OUT_REDIRECT_APPEND:
                    is_out_redirect_append();
                    break;
                case IS_NORMAL_CMD:
                    is_normal_cmd();
                    break;
                default:
                    perror("-MyShell error: command error.\n");
                    break;
            }
            exit(0);
        }   
        else if(pid > 0)
        {
            if(info.flag == IS_PIPE)
            {
                if((chd_pid = fork()) == 0)
                {
                    close(fd[1]);
                    close(0);
                    dup2(fd[0],0);
                    close(fd[0]);
                    if(execvp(info.command2,info.parameters2) == -1)
                    {
                        perror("-MyShell error");
                        exit(1);
                    }
                    else
                        exit(0);
                }
                else
                {
                    close(fd[0]);
                    close(fd[1]);
                    waitpid(chd_pid, &chd_status, 0);
                }
            }
            else if(info.run_flag == IS_BACKGROUND)
            {
                printf("This process is running background. Process id: %d\n",pid);
                continue;
            }
            else
                waitpid(pid, &status, 0);
        }
    }
}
