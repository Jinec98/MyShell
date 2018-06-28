#include "MyShell.h"

int read_command()
{
    char *buffer, *res_fgets;
    buffer = malloc(sizeof(char)*MAXLINE);
    if(buffer == NULL)
        perror("-MyShell error: malloc failed.\n");
    res_fgets = fgets(buffer, MAXLINE, stdin);
    if(res_fgets == NULL)
    {
        printf("-MyShell error: stdin error.\n");
        exit(0);
    }

    if(buffer[0] == '\0')
        return -1;

    char *pStart, *pEnd;
    int count = 0;
    bool isFinished = false;
    pStart = pEnd = buffer;

    while(!isFinished)
    {
        while((*pStart == ' ' && *pEnd == ' ') || (*pStart == '\t' && *pEnd == '\t'))
        {
            pStart++;
            pEnd++;
        }

        if(*pStart == '\0' || *pEnd == '\0')
        {
            if(count == 0)
                return -1;
            break;
        }

        while(*pEnd != ' ' && *pEnd != '\0' && *pEnd != '\n')
            pEnd++;
        
        if(count == 0)
        {
            command = pStart;
            parameters[count] = command;
            count += 2;
        }
        else if(count <= MAXARG)
        {
            parameters[count-1] = pStart;
            count++;
        }
        else
            break;

        if(*pEnd == '\0' || *pEnd == '\n')
        {
            *pEnd = '\0';
            isFinished = true;
        }
        else
        {
            *pEnd = '\0';
            pEnd++;
            pStart = pEnd;
        }
    }
	parameters[count-1] = NULL;
    return count-1;
}
