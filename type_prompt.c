#include "MyShell.h"

void type_prompt()
{
    char hostname[MAX_NAME_LEN];
    char pathname[MAX_PATH_LEN];
    
    if(gethostname(hostname, MAX_NAME_LEN) == 0)
        printf("[%s@%s:", getlogin(), hostname);
    else
        printf("[%s@unknown:", getlogin());
 
    getcwd(pathname, MAX_PATH_LEN);
    if(strlen(pathname) < strlen(path) || strncmp(pathname, path, strlen(path)) != 0)
        printf("%s]", pathname);
    else
        printf("~%s]", pathname+strlen(path));

    if(geteuid() == 0)
        printf("# ");
    else
        printf("$ ");
}
